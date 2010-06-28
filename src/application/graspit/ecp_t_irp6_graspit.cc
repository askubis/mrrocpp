#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include "robot/irp6ot_m/ecp_r_irp6ot_m.h"
#include "robot/irp6p_m/ecp_r_irp6p_m.h"
#include "ecp_t_irp6_graspit.h"
#include "ecp_mp_t_graspit.h"

namespace mrrocpp {
namespace ecp {
namespace common {
namespace task {

//Constructors
irp6_grasp::irp6_grasp(lib::configurator &_config) :
	task(_config)
{

	if (config.section_name == ECP_IRP6OT_M_SECTION) {
		ecp_m_robot = new irp6ot_m::robot(*this);
	} else if (config.section_name == ECP_IRP6P_M_SECTION) {
		ecp_m_robot = new irp6p_m::robot(*this);
	}

	smoothgen2 = new common::generator::smooth(*this, true);
	smoothgen2->sensor_m = sensor_m;

	sr_ecp_msg->message("ecp IRP6 loaded");
}
;

void irp6_grasp::main_task_algorithm(void)
{

	sr_ecp_msg->message("ecp IRP6 ready");

	double v[8], a[8];
	for (int i = 0; i < 8; ++i) {
		v[i] = 0.1;
		a[i] = 0.1;
	}

	for (;;) {
		sr_ecp_msg->message("Waiting for MP order");

		get_next_state();

		sr_ecp_msg->message("Order received");
		flushall();

		if (mp_2_ecp_next_state_string == ecp_mp::task::ECP_GEN_IRP6) {

			sr_ecp_msg->message("ECP_GEN_IRP6");
			sr_ecp_msg->message("Smooth->Move()");
			//				smoothgen2->load_coordinates(lib::ECP_JOINT, v, a,
			//											trgraspit->from_va.graspit.grasp_joint[7],
			//											trgraspit->from_va.graspit.grasp_joint[8],
			//											trgraspit->from_va.graspit.grasp_joint[9],
			//											trgraspit->from_va.graspit.grasp_joint[10],
			//											trgraspit->from_va.graspit.grasp_joint[11],
			//											trgraspit->from_va.graspit.grasp_joint[12],
			//											0.08,
			//											0.0, false);
			//
		} // end switch

		ecp_termination_notice();
	} //end for

	ecp_termination_notice();
}
;

} // namespace task
} // namespace common

namespace common {
namespace task {

task* return_created_ecp_task(lib::configurator &_config)
{
	return new common::task::irp6_grasp(_config);
}

} // namespace task
} // namespace common
} // namespace ecp
} // namespace mrrocpp


