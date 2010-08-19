/*
 * ecp_t_objectfollower_ib.cc
 *
 *  Created on: Apr 21, 2010
 *      Author: mboryn
 */

#include "ecp_t_objectfollower_ib.h"

#include "../ecp_mp_g_visual_servo_tester.h"

using namespace mrrocpp::ecp::common::generator;
using namespace logger;

namespace mrrocpp {

namespace ecp {

namespace common {

namespace task {

const double
		ecp_t_objectfollower_ib::initial_position_joints[MAX_SERVOS_NR] = { 0.0, 0.0, -1.428, 0.0, 0.0, 4.720, 0.0, 0.075 };

ecp_t_objectfollower_ib::ecp_t_objectfollower_ib(mrrocpp::lib::configurator& configurator) :
	task(configurator)
{
	ecp_m_robot = new ecp::irp6p_m::robot(*this);
	//ecp_m_robot = new ecp::irp6ot_m::robot(*this);
	//	smooth_gen = shared_ptr <smooth> (new smooth(*this, true));

	char config_section_name[] = { "[object_follower_ib]" };

	log_dbg_enabled = true;

	shared_ptr <position_constraint> cube(new cubic_constraint(configurator, config_section_name));

	log_dbg("ecp_t_objectfollower_ib::ecp_t_objectfollower_ib(): 1\n");
	reg = shared_ptr <visual_servo_regulator> (new regulator_p(configurator, config_section_name));

	log_dbg("ecp_t_objectfollower_ib::ecp_t_objectfollower_ib(): 2\n");
	vs = shared_ptr <visual_servo> (new ib_eih_visual_servo(reg, config_section_name, configurator));

	term_cond = shared_ptr <termination_condition> (new object_reached_termination_condition(0.005, 0.005, 50));

	log_dbg("ecp_t_objectfollower_ib::ecp_t_objectfollower_ib(): 3\n");
	sm = shared_ptr <simple_visual_servo_manager> (new simple_visual_servo_manager(*this, config_section_name, vs));

	log_dbg("ecp_t_objectfollower_ib::ecp_t_objectfollower_ib(): 4\n");
	sm->add_position_constraint(cube);

	//sm->add_termination_condition(term_cond);
	//log_dbg("ecp_t_objectfollower_ib::ecp_t_objectfollower_ib(): 5\n");

	sm->configure();
	log_dbg("ecp_t_objectfollower_ib::ecp_t_objectfollower_ib(): 6\n");
}

ecp_t_objectfollower_ib::~ecp_t_objectfollower_ib()
{
	delete ecp_m_robot;
}

void ecp_t_objectfollower_ib::main_task_algorithm(void)
{
	while (1) {
		get_next_state();
		if (mp_2_ecp_next_state_string == mrrocpp::ecp_mp::common::generator::ECP_GEN_VISUAL_SERVO_TEST) {
			sm->Move();
		} else {
			log("ecp_t_objectfollower_ib::main_task_algorithm(void) mp_2_ecp_next_state_string: \"%s\"\n", mp_2_ecp_next_state_string.c_str());
		}
	}

	ecp_termination_notice();
}

task* return_created_ecp_task(lib::configurator &config)
{
	return new ecp_t_objectfollower_ib(config);
}

} // namespace task

}//namespace common

}//namespace ecp

}//namespace mrrocpp
