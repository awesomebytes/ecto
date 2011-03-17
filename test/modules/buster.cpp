#include <ecto/ecto.hpp>

#include <iostream>
#include <boost/format.hpp>
#include <boost/foreach.hpp>

//disable show in here
#if 1
#ifdef SHOW
#undef SHOW
#define SHOW() do{}while(false)
#endif
#endif

struct OurModule : ecto::module
{
  static void params(connections_t& c) { }

  OurModule()
  {
    inputs["in"] = ecto::connection::make<int>();
    outputs["out1"] = ecto::connection::make<float>();
    outputs["out2"] = ecto::connection::make<bool>();
  }

  void config() { }
  void process() { }

};

struct Generate : ecto::module
{
  int step_, start_;

  static void params(connections_t& c) { }

  void config(int start, int step)
  {
    //SHOW();
    start_ = start;
    step_ = step;
    setOut<int> ("out", "output", 0);
    setOut<float> ("out02", "What a blast", 0);
  }

  void process()
  {
    //SHOW();
    int& o = getOut<int> ("out");
    o = start_ + step_;
    start_ += step_;
  }
};

struct Multiply : ecto::module
{
  int factor_;

  static void params(connections_t& c)
  {
    c["factor"] = ecto::connection::make<int>(13, "factor", "multiply by...");
  }

  void config(int factor)
  {
    //SHOW();
    factor_ = getParam<int>("factor");
    setIn<int> ("in", "multly in by factor");
    setIn<float> ("fin", "float input");
    setOut<int> ("out", "the result of in * factor");
  }

  void process()
  {
    //SHOW();
    const int& i = getIn<int> ("in");
    int& o = getOut<int> ("out");
    o = i * factor_;
  }
};

std::ostream& operator<<(std::ostream& out, const std::vector<int>& t)
{
  out << "[";
  BOOST_FOREACH(int x, t)
      out << x <<" ";
  return out<<"]";
}

struct Scatter : ecto::module
{
  static void params(connections_t& p) { }

  void config(int x, int n)
  {
    n_ = n;
    x_ = x;
    setOut<std::vector<int> >("out",str(boost::format("A vector, length n=%d, value=%d")%n_ %x_));
  }
  void process()
  {
    SHOW();
    std::vector<int>& out = getOut<std::vector<int> >("out");
    out = std::vector<int>(n_,x_);
  }
  int n_,x_;
};

struct Gather : ecto::module
{
  static void params(connections_t& p) { }

  void config(int n)
  {
    n_ = n;
    for(int i = 0; i < n_; i++)
    {
      setIn<int >(str(boost::format("in_%04d")%i),"An integer input.");
    }
    setOut<int> ("out", "The sum of all inputs.");
  }
  void process()
  {
    SHOW();
    int& out = getOut<int>("out");
    out = 0;
    typedef std::pair<std::string,ecto::connection> pp;
    BOOST_FOREACH(const pp& in,inputs)
    {
      out += in.second.get<int>();
    }
  }
  int n_;
};

struct Indexer : ecto::module
{
  static void params(connections_t& p) { }

  void config(int n)
  {
    n_ = n;
    setIn<std::vector<int> >("in", "the vector to index",std::vector<int>(n_));
    setOut<int >("out",str(boost::format("the output at in[%d]")%n_),0);
  }
  void process()
  {
    SHOW();
    const std::vector<int>& in = getIn<std::vector<int> >("in");
    int & out = getOut<int>("out");
    if(in.size() > size_t( n_))
    out = in[n_];
    else
      std::cerr << "wrong size! idx= " << n_ << " length of vector " << in.size() << std::endl;
  }
  int n_;
};

ECTO_MODULE(buster)
{
  ecto::wrap<OurModule>("OurModule");
  ecto::wrap<Generate>("Generate");
  ecto::wrap<Multiply>("Multiply");
  ecto::wrap<Scatter>("Scatter");
  ecto::wrap<Indexer>("Indexer");
  ecto::wrap<Gather>("Gather");
}
