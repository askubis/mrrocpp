/*
 * generator/ecp_g_eihcalibration.cc
 *
 *  Created on: July 28, 2009
 *      Author: jkosiore
 */

#include "ecp_g_eihcalibration.h"

namespace mrrocpp {
namespace ecp {
namespace common {
namespace generator {

using namespace std;


eihgenerator::eihgenerator (common::task::task& _ecp_task)
        : generator (_ecp_task)
{
	count = -1;
}


eihgenerator::~eihgenerator ()
{

}

bool eihgenerator::first_step()
{
	sensor = dynamic_cast<ecp_mp::sensor::fradia_sensor<chessboard_t, eihcalibration_t> *> (sensor_m[lib::SENSOR_CVFRADIA]);

	//proste zadanie kinematyki
	the_robot->ecp_command.instruction.instruction_type = lib::GET;
	the_robot->ecp_command.instruction.get_type = ARM_DEFINITION;
	the_robot->ecp_command.instruction.get_arm_type = lib::FRAME;

	//sensor->to_vsp.i_code = lib::VSP_INITIATE_READING;

	printf("bool eihgenerator::first_step()\n"); fflush(stdout);
	return true;
}

bool eihgenerator::next_step()
{
	printf("bool eihgenerator::next_step()\n"); fflush(stdout);
	float t[12];
	if(sensor->image.found == true)
		count++;
	get_frame();
	eihcalibration_t command;
	command.frame_number = count;
	sensor->configure_fradia_task(command);
	//sensor->to_vsp.parameters.frame_number = count;
	return false;
}

void eihgenerator::get_frame()
{
/*	std::cout.precision(3);
	std::cout.width(6);
	std::cout.setf(ios::fixed,ios::floatfield);
*/	for(int i=0; i<3; i++)
	{
		for(int j=0; j<4; j++)
		{
			tab[4*i+j] = the_robot->reply_package.arm.pf_def.arm_frame[i][j];
			//std::cout << t[4*i+j] << "\t";
		}
		//std::cout<<std::endl;
	}
	//std::cout<<std::endl;

}


}
} // namespace common
} // namespace ecp
} // namespace mrrocpp


