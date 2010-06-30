/**
 * \file single_thread_port.h
 *
 * \date 2010
 * \author yoyek
 *
 * \brief Template class for a single robot communication port
 */

#ifndef SINGLE_THREAD_PORT_H_
#define SINGLE_THREAD_PORT_H_

#include <string>
#include <map>

#include <boost/cast.hpp>

namespace mrrocpp {
namespace lib {

enum FlowStatus
{
	NoData, OldData, NewData
};

class single_thread_port_interface
{
private:
	const std::string name;

protected:
	bool new_data;

public:
	single_thread_port_interface(const std::string & _name) :
		name(_name), new_data(false)
	{
	}

	std::string get_name() const
	{
		return name;
	}

	/**
	 * This is a base class, so virtual destructor is recommended
	 * and it is also required for dynamic casting.
	 */
	virtual ~single_thread_port_interface()
	{
	}
};

template <class T>
class single_thread_port : public single_thread_port_interface
{
protected:
	bool no_data;
	T data;

public:
	single_thread_port(const std::string & _name) :
		single_thread_port_interface(_name), no_data(true)
	{
	}

	virtual void set(const T& _data)
	{
		data = _data;
		no_data = false;
		new_data = true;
	}

	virtual FlowStatus get(T& _data)
	{
		if (no_data) {
			return NoData;
		} else if (new_data) {
			_data = data;
			new_data = false;
			return NewData;
		} else {
			_data = data;
			return OldData;
		}
	}

	void clear_new_data_flag()
	{
		new_data = false;
	}
};

template <class T>
class single_thread_request_port : public single_thread_port <T>
{
protected:
	bool new_request;

public:
	single_thread_request_port(const std::string & _name) :
		single_thread_port <T> (_name), new_request(false)
	{
	}

	void set_request()
	{
		new_request = true;
	}

	void set(const T& _data)
	{
		new_request = false;
		single_thread_port <T>::set(_data);
	}

	bool is_new_request() const
	{
		return new_request;
	}

	void clear_new_request_flag()
	{
		new_request = false;
	}
};

class single_thread_port_manager
{
private:
	std::map <std::string, single_thread_port_interface *> single_thread_port_map;

public:
	single_thread_port_manager()
	{
	}

	void add_port(single_thread_port_interface* single_thread_port_inter)
	{
		single_thread_port_map[single_thread_port_inter->get_name()] = single_thread_port_inter;
	}

	template <class T>
	single_thread_port <T>* get_port(const std::string & name)
	{
		return boost::polymorphic_cast<single_thread_port <T> *> (single_thread_port_map[name]);
	}

	template <class T>
	single_thread_request_port <T>* get_request_port(const std::string & name)
	{
		return boost::polymorphic_cast<single_thread_request_port <T> *> (single_thread_port_map[name]);
	}
};

}
}

#endif /* SINGLE_THREAD_PORT_H_ */
