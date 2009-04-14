#ifndef ECP_MP_GENERATOR_H_
#define ECP_MP_GENERATOR_H_

#include <map>
#include "lib/srlib.h"
#include "lib/sensor.h"
#include "ecp_mp/transmitter.h"

namespace mrrocpp {
namespace ecp_mp {
namespace generator {



class base
{
	// Klasa bazowa dla generatorow trajektorii (klasa abstrakcyjna)
	// Sluzy zarowno do wyznaczania nastepnej wartosci zadanej jak i
	// sprawdzania spelnienia warunku koncowego

protected:
	lib::sr_ecp& sr_ecp_msg;		// obiekt do komunikacji z SR

public:
	bool trigger;			// informacja czy pszyszedl puls trigger

	base(lib::sr_ecp& _sr_ecp_msg);

	bool check_and_null_trigger(); 		// zwraca wartosc trigger i zeruje go

	int node_counter;  // biezacy wezel interpolacji

	virtual ~base();

	// mapa wszystkich czujnikow
	std::map <lib::SENSOR_ENUM, lib::sensor*> sensor_m;

	// mapa wszystkich transmiterow
	std::map <transmitter::TRANSMITTER_ENUM, transmitter::base*> transmitter_m;

	// generuje pierwszy krok ruchu -
	// pierwszy krok czesto rozni sie od pozostalych,
	// np. do jego generacji nie wykorzystuje sie czujnikow
	// (zadanie realizowane przez klase konkretna)
	virtual bool first_step (void) = 0;

	// generuje kazdy nastepny krok ruchu
	// (zadanie realizowane przez klase konkretna)
	virtual bool next_step (void) = 0;
};

} // namespace generatora
} // namespace ecp_mp
} // namespace mrrocpp

#endif /*ECP_MP_GENERATOR_H_*/
