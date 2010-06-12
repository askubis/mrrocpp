// -------------------------------------------------------------------------
// Proces: 	EFFECTOR CONTROL PROCESS (lib::ECP)
// Plik:			configsrv.h
// System:	QNX/MRROCPP  v. 6.3
// Opis:		Plik zawiera klase configsrv - obsluga konfiguracji z pliku INI.
// Autor:		tkornuta
// Data:		10.11.2005
// -------------------------------------------------------------------------


#if !defined(_CONFIGSRV_H)
#define _CONFIGSRV_H

#include <boost/thread/mutex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>

class configsrv
{
private:
	const std::string node, dir;
	std::string ini_file, mrrocpp_network_path;
	std::string file_location, common_file_location;

	// do ochrony wylacznosci dostepu do pliku miedzy watkami jednego procesu
	boost::mutex mtx;

	//! Property tree of configuration file
	boost::property_tree::ptree file_pt;

	/**
	 * Read property tree from configuration file
	 * @param pt property tree
	 * @param file configuration file
	 */
	void read_property_tree_from_file(boost::property_tree::ptree & pt, const std::string & file);

public:
	// Konstruktor obiektu - konfiguratora.
	configsrv(const std::string & _node, const std::string & _dir, const std::string & _ini_file);

	void change_ini_file(const std::string & _ini_file);

	// Zwraca wartosc dla klucza.
	template <class Type>
	Type value(const std::string & _key, const std::string & __section_name) const
	{
		// initialize property tree path
		std::string pt_path = __section_name;

		// trim leading '[' char
		pt_path.erase(0, 1);
		// trim trailing '[' char
		pt_path.erase(pt_path.length() - 1, 1);

		pt_path += ".";
		pt_path += _key;

		return file_pt.get <Type> (pt_path);
	}

	// Zwraca czy dany klucz istnieje
	bool exists(const std::string & _key, const std::string & _section_name);
};

#endif /* _CONFIGSRV_H */
