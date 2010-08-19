/*
 * generator/ecp_g_bird_hand.cc
 *
 *Author: yoyek
 */

#include "ecp_g_birdhand_graspit.h"

namespace mrrocpp {
namespace ecp {
namespace common {
namespace generator {

//constructor with parameters: task and time to sleep [s]
bird_hand::bird_hand(common::task::task& _ecp_task) :
	generator(_ecp_task), MAX_V(8000.0 / 275.0 / 7.826 / 60.0 / 1000.0), STEP_NO(50) {
	bird_hand_command_data_port = the_robot->port_manager.get_port<
			lib::bird_hand_command> (BIRD_HAND_COMMAND_DATA_PORT);

	bird_hand_configuration_command_data_port
			= the_robot->port_manager.get_port<lib::bird_hand_configuration> (
					BIRD_HAND_CONFIGURATION_DATA_PORT);

	bird_hand_status_reply_data_request_port
			= the_robot->port_manager.get_request_port<lib::bird_hand_status> (
					BIRD_HAND_STATUS_DATA_REQUEST_PORT);

	bird_hand_configuration_reply_data_request_port
			= the_robot->port_manager.get_request_port<
					lib::bird_hand_configuration> (
					BIRD_HAND_CONFIGURATION_DATA_REQUEST_PORT);
}

void bird_hand::create_ecp_mp_reply() {
}

void bird_hand::get_mp_ecp_command() {

	memcpy(&bird_hand_command_structure,
			ecp_t.mp_command.ecp_next_state.mp_2_ecp_next_state_string,
			sizeof(bird_hand_command_structure));
}

bool bird_hand::first_step() {

	// parameters copying
	get_mp_ecp_command();

	//bird_hand_configuration_command_data_port->set(bird_hand_configuration_command_structure);

	bird_hand_command_structure.thumb_f[0].profile_type = mrrocpp::lib::BIRD_HAND_MACROSTEP_POSITION_INCREMENT;
	bird_hand_command_structure.thumb_f[1].profile_type = mrrocpp::lib::BIRD_HAND_MACROSTEP_POSITION_INCREMENT;
	bird_hand_command_structure.index_f[0].profile_type = mrrocpp::lib::BIRD_HAND_MACROSTEP_POSITION_INCREMENT;
	bird_hand_command_structure.index_f[1].profile_type = mrrocpp::lib::BIRD_HAND_MACROSTEP_POSITION_INCREMENT;
	bird_hand_command_structure.index_f[2].profile_type = mrrocpp::lib::BIRD_HAND_MACROSTEP_POSITION_INCREMENT;
	bird_hand_command_structure.ring_f[0].profile_type = mrrocpp::lib::BIRD_HAND_MACROSTEP_POSITION_INCREMENT;
	bird_hand_command_structure.ring_f[1].profile_type = mrrocpp::lib::BIRD_HAND_MACROSTEP_POSITION_INCREMENT;
	bird_hand_command_structure.ring_f[2].profile_type = mrrocpp::lib::BIRD_HAND_MACROSTEP_POSITION_INCREMENT;

	bird_hand_command_structure.thumb_f[0].desired_torque = 0;
	bird_hand_command_structure.thumb_f[1].desired_torque = 0;
	bird_hand_command_structure.index_f[0].desired_torque = 0;
	bird_hand_command_structure.index_f[1].desired_torque = 0;
	bird_hand_command_structure.index_f[2].desired_torque = 0;
	bird_hand_command_structure.ring_f[0].desired_torque = 0;
	bird_hand_command_structure.ring_f[1].desired_torque = 0;
	bird_hand_command_structure.ring_f[2].desired_torque = 0;

	bird_hand_command_structure.thumb_f[0].reciprocal_of_damping = 0;
	bird_hand_command_structure.thumb_f[1].reciprocal_of_damping = 0;
	bird_hand_command_structure.index_f[0].reciprocal_of_damping = 0;
	bird_hand_command_structure.index_f[1].reciprocal_of_damping = 0;
	bird_hand_command_structure.index_f[2].reciprocal_of_damping = 0;
	bird_hand_command_structure.ring_f[0].reciprocal_of_damping = 0;
	bird_hand_command_structure.ring_f[1].reciprocal_of_damping = 0;
	bird_hand_command_structure.ring_f[2].reciprocal_of_damping = 0;

	des_thumb_f[0] = bird_hand_command_structure.thumb_f[0].desired_position;
	des_thumb_f[1] = bird_hand_command_structure.thumb_f[1].desired_position;
	des_index_f[0] = bird_hand_command_structure.index_f[0].desired_position;
	des_index_f[1] = bird_hand_command_structure.index_f[1].desired_position;
	des_index_f[2] = bird_hand_command_structure.index_f[2].desired_position;
	des_ring_f[0] = bird_hand_command_structure.ring_f[0].desired_position;
	des_ring_f[1] = bird_hand_command_structure.ring_f[1].desired_position;
	des_ring_f[2] = bird_hand_command_structure.ring_f[2].desired_position;

	//pierwszy next_step pusty
	bird_hand_command_structure.thumb_f[0].desired_position = 0.0;
	bird_hand_command_structure.thumb_f[1].desired_position = 0.0;
	bird_hand_command_structure.index_f[0].desired_position = 0.0;
	bird_hand_command_structure.index_f[1].desired_position = 0.0;
	bird_hand_command_structure.index_f[2].desired_position = 0.0;
	bird_hand_command_structure.ring_f[0].desired_position = 0.0;
	bird_hand_command_structure.ring_f[1].desired_position = 0.0;
	bird_hand_command_structure.ring_f[2].desired_position = 0.0;

	bird_hand_command_structure.motion_steps = STEP_NO;
	bird_hand_command_structure.ecp_query_step = STEP_NO - 3;

	bird_hand_command_data_port->set(bird_hand_command_structure);
	bird_hand_status_reply_data_request_port->set_request();

	return true;
}

bool bird_hand::next_step() {

	if (bird_hand_status_reply_data_request_port->get(
			bird_hand_status_reply_structure) == mrrocpp::lib::NewData) {

		std::cout << "\n node_counter: " << node_counter;

		if (node_counter == 1) {
			double max_dist = fabs(bird_hand_status_reply_structure.thumb_f[0].meassured_position - des_thumb_f[0]);
			if (fabs(bird_hand_status_reply_structure.thumb_f[1].meassured_position - des_thumb_f[1]) > max_dist)
				max_dist = fabs(bird_hand_status_reply_structure.thumb_f[1].meassured_position - des_thumb_f[1]);
			for (int i=0; i<3; ++i)
				if (fabs(bird_hand_status_reply_structure.index_f[i].meassured_position - des_index_f[i]) > max_dist)
					max_dist = fabs(bird_hand_status_reply_structure.index_f[i].meassured_position - des_index_f[i]);
			for (int i=0; i<3; ++i)
				if (fabs(bird_hand_status_reply_structure.ring_f[i].meassured_position - des_ring_f[i]) > max_dist)
					max_dist = fabs(bird_hand_status_reply_structure.ring_f[i].meassured_position - des_ring_f[i]);

			if (max_dist == 0.0) {
				std::cout << "\n max_dist: " << max_dist;
				return false;
			}

			double time = max_dist / MAX_V;

			last_step = fmod(time, STEP_NO);
			macro_no = (time - last_step) / STEP_NO;
			//+1 dla last_step
			++macro_no;

			//ostatni makrokrok dluzszy
			last_step += STEP_NO;

			bird_hand_command_structure.thumb_f[0].desired_position = des_thumb_f[0] / macro_no;
			bird_hand_command_structure.thumb_f[1].desired_position = des_thumb_f[1] / macro_no;
			bird_hand_command_structure.index_f[0].desired_position = des_index_f[0] / macro_no;
			bird_hand_command_structure.index_f[1].desired_position = des_index_f[1] / macro_no;
			bird_hand_command_structure.index_f[2].desired_position = des_index_f[2] / macro_no;
			bird_hand_command_structure.ring_f[0].desired_position = des_ring_f[0] / macro_no;
			bird_hand_command_structure.ring_f[1].desired_position = des_ring_f[1] / macro_no;
			bird_hand_command_structure.ring_f[2].desired_position = des_ring_f[2] / macro_no;

			std::cout << "\n macro_no: " << macro_no;
			std::cout << "\n last_step: " << last_step;
			std::cout << "\n time: " << time;
			std::cout << "\n max_dist: " << max_dist;
			std::cout << "\n max_v: " << MAX_V;
		}

//		std::cout << "\n thumb_f[0].meassured_position: " << bird_hand_status_reply_structure.thumb_f[0].meassured_position;
//		std::cout << "\n thumb_f[1].meassured_position: " << bird_hand_status_reply_structure.thumb_f[1].meassured_position;
//		std::cout << "\n index_f[0].meassured_position: " << bird_hand_status_reply_structure.index_f[0].meassured_position;
//		std::cout << "\n index_f[1].meassured_position: " << bird_hand_status_reply_structure.index_f[1].meassured_position;
//		std::cout << "\n index_f[2].meassured_position: " << bird_hand_status_reply_structure.index_f[2].meassured_position;
//		std::cout << "\n ring_f[0].meassured_position: " << bird_hand_status_reply_structure.ring_f[0].meassured_position;
//		std::cout << "\n ring_f[1].meassured_position: " << bird_hand_status_reply_structure.ring_f[1].meassured_position;
//		std::cout << "\n ring_f[2].meassured_position: " << bird_hand_status_reply_structure.ring_f[2].meassured_position;
	}

	bird_hand_configuration_reply_data_request_port->get(
			bird_hand_configuration_reply_structure);

	bird_hand_command_data_port->set(bird_hand_command_structure);
	bird_hand_status_reply_data_request_port->set_request();
	bird_hand_configuration_reply_data_request_port->set_request();

	if (node_counter <= macro_no)
		return true;
	else
		if (node_counter == macro_no + 1) {
			bird_hand_command_structure.motion_steps = last_step;
			bird_hand_command_structure.ecp_query_step = last_step - 3;
			return true;
		}
	return false;

}

} // namespace generator
} // namespace common
} // namespace ecp
} // namespace mrrocpp

