#include <stdio.h>

#include "lib/typedefs.h"
#include "lib/impconst.h"
#include "lib/com_buf.h"

#include "lib/srlib.h"
#include "ecp_mp_t_swarmitfix.h"

#include "ecp/spkm/ecp_r_spkm.h"
#include "ecp/common/generator/ecp_g_smooth.h"
#include "ecp/common/generator/ecp_g_sleep.h"
#include "ecp_g_epos.h"
#include "ecp_t_spkm.h"

namespace mrrocpp {
namespace ecp {
namespace spkm {
namespace task {

// KONSTRUKTORY
swarmitfix::swarmitfix(lib::configurator &_config) :
	task(_config)
{
	// the robot is choose dependendat on the section of configuration file sent as argv[4]
	ecp_m_robot = new robot(*this);

	gt = new common::generator::transparent(*this);
	sg = new common::generator::smooth(*this, true);
	g_sleep = new common::generator::sleep(*this);
	g_epos = new common::generator::epos(*this);

	sr_ecp_msg->message("ECP spkm loaded");
}

void swarmitfix::main_task_algorithm(void)
{
	for (;;) {
		sr_ecp_msg->message("Waiting for MP order");

		get_next_state();

		sr_ecp_msg->message("Order received");
		//printf("postument: %d\n", mp_command.ecp_next_state.mp_2_ecp_next_state);
		//flushall();

		if (mp_2_ecp_next_state_string == ecp_mp::task::ECP_GEN_TRANSPARENT) {
			gt->throw_kinematics_exceptions = (bool) mp_command.ecp_next_state.mp_2_ecp_next_state_variant;
			gt->Move();

		} else if (mp_2_ecp_next_state_string == ecp_mp::task::ECP_GEN_SMOOTH) {
			std::string path(mrrocpp_network_path);
			path += mp_command.ecp_next_state.mp_2_ecp_next_state_string;

			switch ((ecp_mp::task::SMOOTH_MOTION_TYPE) mp_command.ecp_next_state.mp_2_ecp_next_state_variant)
			{
				case ecp_mp::task::RELATIVE:
					sg->set_relative();
					break;
				case ecp_mp::task::ABSOLUTE:
					sg->set_absolute();
					break;
				default:
					break;
			}

			sg->load_file_with_path(path.c_str());
			sg->Move();

		} else if (mp_2_ecp_next_state_string == ecp_mp::task::ECP_GEN_SLEEP) {

			g_sleep->init_time(mp_command.ecp_next_state.mp_2_ecp_next_state_variant);
			g_sleep->Move();
		} else if (mp_2_ecp_next_state_string == ecp_mp::task::ECP_GEN_EPOS) {
			sr_ecp_msg->message("ECP_GEN_EPOS");

			g_epos->Move();

		}

		ecp_termination_notice();
	} //end for
}

}
} // namespace spkm

namespace common {
namespace task {

task* return_created_ecp_task(lib::configurator &_config)
{
	return new spkm::task::swarmitfix(_config);
}

}
} // namespace common
} // namespace ecp
} // namespace mrrocpp
