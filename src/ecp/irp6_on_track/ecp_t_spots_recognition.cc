/*!
 * \file ecp_t_spots_recognition.cc
 * \brief Piotr Sakowicz's methods responsible
 * for robot moving while external calibration.
 * - methods definitions
 * \author vented.baffle
 * \date 21.08.2008
 */

#include "ecp/irp6_on_track/ecp_t_spots_recognition.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

namespace mrrocpp {
namespace ecp {
namespace irp6ot {
namespace task {

//own libraries
//Constructors
spots_recognition::spots_recognition(lib::configurator &_config): base(_config)
{
    trajektoria_poczatkowa = "../trj/spots/traj00.trj";
    remove("../msr/kalibracja.txt");
}

spots_recognition::~spots_recognition()
{

}

// methods for ECP template to redefine in concrete classes
void spots_recognition::task_initialization(void)
{

	// Create cvFraDIA sensor - for testing purposes.
	sensor_m[lib::SENSOR_CVFRADIA] = new ecp_mp::sensor::cvfradia(lib::SENSOR_CVFRADIA, "[vsp_cvfradia]", *this, sizeof(lib::sensor_image_t::sensor_union_t::sp_r_t));
	// Configure sensor.
	sensor_m[lib::SENSOR_CVFRADIA]->configure_sensor();

// Create an adequate robot. - depending on the ini section name.
// Note - only on track working
if (strcmp(config.section_name, "[ecp_irp6_on_track]") == 0)
{
    ecp_m_robot = new ecp_irp6_on_track_robot (*this);
    sr_ecp_msg->message("IRp6 on Track loaded");
}
/*else if (strcmp(config.section_name, "[ecp_irp6_postument]") == 0)
{
    ecp_m_robot = new ecp_irp6_postument_robot (*this);
    sr_ecp_msg->message("IRp6 Postument loaded");
}*/
	// Create spots generator and pass sensor to it.
	generator = new generator::spots(*this);
 	generator->sensor_m = sensor_m;

	// Create smooth generator.
	smooth = new common::generator::smooth(*this, true, false);

	nose = new common::generator::y_nose_run_force(*this);

}

void spots_recognition::main_task_algorithm(void)
{
	/*!
	    	 * smooth generator odczytuje trajektorie z pliku
	    	 * po czym przesuwa sie do poczatkowej pozycji
	 */

	smooth->load_file_with_path(trajektoria_poczatkowa);
	smooth->Move();

    for(int i=0; i<=15; i++)
    {



	    /*!
	     * nastepnie nalezy odczekac ok 1s, zanim mozna rozpoczac sesje zdjeciowa
	     */
	    sleep(2);

	    /*!
	     * zrob zdjecia, dokonaj obliczen
	     */
	    generator->Move();

    }
}

}
} // namespace irp6ot

namespace common {
namespace task {

base* return_created_ecp_task(lib::configurator &_config)
{
	return new irp6ot::task::spots_recognition(_config);
}

}
} // namespace common
} // namespace ecp
} // namespace mrrocpp


