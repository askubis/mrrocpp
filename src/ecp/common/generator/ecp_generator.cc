#include "ecp/common/generator/ecp_generator.h"

#include "lib/exception.h"
#include <boost/throw_exception.hpp>
#include <boost/exception/info.hpp>

namespace mrrocpp {
namespace ecp {
namespace common {
namespace generator {

generator::generator(common::task::task& _ecp_task) :
	ecp_mp::generator::generator(*(ecp_t.sr_ecp_msg)), ecp_t(_ecp_task),
			communicate_with_mp_in_move(true), the_robot(ecp_t.ecp_m_robot)
{
}

generator::~generator() {
}

bool generator::is_EDP_error(const ecp_robot& _robot) const {
	// Sprawdzenie czy nie wystapil blad w EDP
	// Funkcja zaklada, ze error_no zostalo zaktualizowane
	// za pomoca conveyor_generator::get_reply
	if (_robot.reply_package.error_no.error0
			|| _robot.reply_package.error_no.error1) {
		return true;
	} else {
		return false;
	}
}

void generator::move_init()
{
	// domyslnie komunikumemy sie z robotem o ile on jest
	if (the_robot)
	{
		// default communication mode
		the_robot->communicate_with_edp = true;
		// clear data ports in case there is old data in it;
		the_robot->clear_data_ports();
	}
	// domyslny tryb koordynacji
	ecp_t.continuous_coordination = false;

	// generacja pierwszego kroku ruchu
	node_counter = 0;
	ecp_t.set_ecp_reply(lib::ECP_ACKNOWLEDGE);
}

void generator::Move() {
	// Funkcja ruchu dla ECP

	move_init();

	if (!first_step()) {
		return; // Warunek koncowy spelniony w pierwszym kroku
	}

	do { // realizacja ruchu

		// zadanie przygotowania danych od czujnikow
		ecp_t.all_sensors_initiate_reading(sensor_m);

		// Check if there is EDP to control
		// (in some simplest cases there is no EDP)
		if (the_robot) {

			// If it is not continuous coordination case, where MP
			// commands the EDP directly - we have to create command
			// for the robot
			if (!ecp_t.continuous_coordination) {

				the_robot->create_command();
			}

			// Execute motion command
			if (the_robot->communicate_with_edp) {
				execute_motion();

				the_robot->get_reply();
			}
		}

		// odczytanie danych z wszystkich czujnikow
		ecp_t.all_sensors_get_reading(sensor_m);

		node_counter++;

		// ECP synchronizes isself at query() to EDP
		ecp_t.Wait();

		ecp_t.mp_command = ecp_t.mp_command_buffer.Get();

		/*
		if (ecp_t.pulse_check()) {
			trigger = true;
		}
		*/

	} while (next_step());
	//(next_step() && (!communicate_with_mp_in_move || ecp_t.mp_buffer_receive_and_send()));
}

void generator::execute_motion(void) {
	the_robot->execute_motion();
}

} // namespace generator
} // namespace common
} // namespace ecp
} // namespace mrrocpp


