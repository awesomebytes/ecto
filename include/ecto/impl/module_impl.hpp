template<typename T>
tendril& module::setOut(const std::string& name,
                      const std::string& doc,
                      const T& t)
{
  outputs[name] = ecto::tendril::make<T>(t,name,doc);
  return outputs[name];
}

template<typename T>
tendril& 
module::setIn(const std::string& name,
	      const std::string& doc,
	      const T& t)
{
  inputs[name] = ecto::tendril::make<T>(t,name,doc);
  return inputs[name];
}

template<typename T>
tendril& 
module::setParam(const std::string& name,
		 const std::string& doc,
		 const T& t)
{
  params[name] = ecto::tendril::make<T>(t,name,doc);
  return params[name];
}

template<typename T>
T& module::getOut(const std::string& name)
{
  //FIXME check for null, throw
  return outputs[name].get<T>();
}
template<typename T>
const T& module::getIn(const std::string& name)
{
  //FIXME check for null, throw
  return inputs[name].get<T>();
}

template<typename T>
const T& module::getParam(const std::string& name)
{
  //FIXME check for null, throw
  return params[name].get<T>();
}
