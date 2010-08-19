/**
 * \file ecp_g_newsmooth.cc
 * \brief newsmooth class and its methods
 *
 * Contains bodies of the methods of newsmooth class.
 */

#include "generator/ecp/ecp_g_newsmooth.h"

namespace mrrocpp {
namespace ecp {
namespace common {
namespace generator {

newsmooth::newsmooth(common::task::task& _ecp_task, lib::ECP_POSE_SPECIFICATION pose_spec, int axes_num) :
				multiple_position<ecp_mp::common::trajectory_pose::bang_bang_trajectory_pose,
				ecp::common::generator::trajectory_interpolator::bang_bang_interpolator,
				ecp::common::generator::velocity_profile_calculator::bang_bang_profile> (_ecp_task) {
	this->pose_spec = pose_spec;
	this->axes_num = axes_num;
	this->vpc = velocity_profile_calculator::bang_bang_profile();
	this->inter = trajectory_interpolator::bang_bang_interpolator();

	create_velocity_vectors(axes_num);
}

newsmooth::~newsmooth() {

}

bool newsmooth::calculate() {
	//printf("\n################################## Calculate #################################\n");
	sr_ecp_msg.message("Calculating...");
	int i,j;//loop counters

	pose_vector_iterator = pose_vector.begin();

	for (i = 0; i < pose_vector.size(); i++) {
		if (!vpc.calculate_v_r_a_r_pose(pose_vector_iterator)) {
			return false;
		}
		pose_vector_iterator++;
	}


	//------------- testowo ----------
	/*pose_vector_iterator = pose_vector.begin();

	for (i = 0; i < pose_vector.size(); i++) {
		if (pose_spec == lib::ECP_XYZ_ANGLE_AXIS) {
			set_relative();
			if (!vpc.calculate_relative_angle_axis_vector(pose_vector_iterator)) {
				return false;
			}
		}
		pose_vector_iterator++;
	}*/
	//------------ testowo end ---------

	pose_vector_iterator = pose_vector.begin();

	for (i = 0; i < pose_vector.size(); i++) {//calculate distances, directions

		if(motion_type == lib::ABSOLUTE) {//absolute type of motion
			if (!vpc.calculate_absolute_distance_direction_pose(pose_vector_iterator)) {
				return false;
			}
		} else if(motion_type == lib::RELATIVE) {//relative type of motion
			if (!vpc.calculate_relative_distance_direction_pose(pose_vector_iterator)) {
				return false;
			}
		} else {
			sr_ecp_msg.message("Wrong motion type");
			throw ECP_error(lib::NON_FATAL_ERROR, ECP_ERRORS);//TODO change the second argument
		}

		pose_vector_iterator++;
	}

	pose_vector_iterator = pose_vector.begin();

	vector<ecp_mp::common::trajectory_pose::bang_bang_trajectory_pose>::iterator tempIter = pose_vector.end();
	vector<ecp_mp::common::trajectory_pose::bang_bang_trajectory_pose>::iterator tempIter2 = pose_vector.begin();

	for (i = 0; i < pose_vector.size(); i++) { //for each pose

		if (!vpc.set_v_k_pose(pose_vector_iterator, tempIter) ||//set up v_k for the pose
		!vpc.set_v_p_pose(pose_vector_iterator, tempIter2) ||//set up v_p for the pose
		!vpc.set_model_pose(pose_vector_iterator) || //choose motion model for the pose
		!vpc.calculate_s_acc_s_dec_pose(pose_vector_iterator)) { //calculate s_acc and s_dec for the pose
			return false;
		}
		//printf("\n------------ first print pose %d --------------\n", pose_vector_iterator->pos_num);
		//print_pose(pose_vector_iterator);

		for(j = 0; j < axes_num; j++) { //for each axis
			if (vpc.check_if_no_movement(pose_vector_iterator, j)) {
				continue;
			}

			if (vpc.check_s_acc_s_decc(pose_vector_iterator, j)) {//check if s_acc && s_dec < s
				vpc.calculate_s_uni(pose_vector_iterator, j);//calculate s_uni
				vpc.calculate_time(pose_vector_iterator, j);//calculate and set time
			} else{//if not
				if(!vpc.optimize_time_axis(pose_vector_iterator, j)) {
					return calculate();
				}

				if(!vpc.reduction_axis(pose_vector_iterator, j)) {
					return calculate();
				}
			}
		}

		if (!vpc.set_model_pose(pose_vector_iterator)) {
			return false;
		}

		if (!vpc.calculate_pose_time(pose_vector_iterator, mc) ||//calculate pose time
			!vpc.set_times_to_t(pose_vector_iterator)) {//set times to t
			return false;
		}

		//printf("\n------------ second print pose %d --------------\n", pose_vector_iterator->pos_num);
		//print_pose(pose_vector_iterator);

		pose_vector_iterator->interpolation_node_no = ceil(pose_vector_iterator->t / mc);//calculate the number of the macrosteps for the pose

		for(j = 0; j < axes_num; j++) {//for each axis call reduction methods
			if(!vpc.reduction_axis(pose_vector_iterator, j)) {
				return calculate();
			}
		}

		//printf("\n------------ third print pose %d --------------\n", pose_vector_iterator->pos_num);
		//print_pose(pose_vector_iterator);

		if (!vpc.calculate_acc_uni_pose(pose_vector_iterator, mc)) {//set uni and acc
			return false;
		}

		pose_vector_iterator++;
	}

	return true;
}

void newsmooth::print_pose_vector() {
	printf("\n------------------ Pose List ------------------\n");
	pose_vector_iterator = pose_vector.begin();
	for (int k = 0; k < pose_vector.size(); k++) {
		print_pose(pose_vector_iterator);
		pose_vector_iterator++;
	}
}

void newsmooth::print_pose(vector<ecp_mp::common::trajectory_pose::bang_bang_trajectory_pose>::iterator & it) {
	int z;
	printf("s:\t");
	for (z = 0; z < pose_vector_iterator->s.size(); z++) {
		printf("%f\t", pose_vector_iterator->s[z]);
	}
	printf("\n");
	printf("k:\t");
	for (z = 0; z < pose_vector_iterator->k.size(); z++) {
		printf("%f\t", pose_vector_iterator->k[z]);
	}
	printf("\n");
	printf("times:\t");
	for (z = 0; z < pose_vector_iterator->s.size(); z++) {
		printf("%f\t", pose_vector_iterator->times[z]);
	}
	printf("\n");
	printf("v_r:\t");
	for (z = 0; z < pose_vector_iterator->v_r.size(); z++) {
		printf("%f\t", pose_vector_iterator->v_r[z]);
	}
	printf("\n");
	printf("a_r:\t");
	for (z = 0; z < pose_vector_iterator->a_r.size(); z++) {
		printf("%f\t", pose_vector_iterator->a_r[z]);
	}
	printf("\n");
	printf("v_p:\t");
	for (z = 0; z < pose_vector_iterator->v_p.size(); z++) {
		printf("%f\t", pose_vector_iterator->v_p[z]);
	}
	printf("\n");
	printf("v_k:\t");
	for (z = 0; z < pose_vector_iterator->v_k.size(); z++) {
		printf("%f\t", pose_vector_iterator->v_k[z]);
	}
	printf("\n");
	printf("s_acc:\t");
	for (z = 0; z < pose_vector_iterator->s_acc.size(); z++) {
		printf("%f\t", pose_vector_iterator->s_acc[z]);
	}
	printf("\n");
	printf("s_uni:\t");
	for (z = 0; z < pose_vector_iterator->s_uni.size(); z++) {
		printf("%f\t", pose_vector_iterator->s_uni[z]);
	}
	printf("\n");
	printf("s_dec:\t");
	for (z = 0; z < pose_vector_iterator->s_dec.size(); z++) {
		printf("%f\t", pose_vector_iterator->s_dec[z]);
	}
	printf("\n");
	printf("model:\t");
	for (z = 0; z < pose_vector_iterator->model.size(); z++) {
		printf("%d\t\t", pose_vector_iterator->model[z]);
	}
	printf("\n");
	printf("acc:\t");
	for (z = 0; z < pose_vector_iterator->acc.size(); z++) {
		printf("%f\t", pose_vector_iterator->acc[z]);
	}
	printf("\n");
	printf("uni:\t");
	for (z = 0; z < pose_vector_iterator->uni.size(); z++) {
		printf("%f\t", pose_vector_iterator->uni[z]);
	}
	printf("\n");
	printf("t: %f\t pos_num: %d\t number of macrosteps: %d\n", pose_vector_iterator->t, pose_vector_iterator->pos_num, pose_vector_iterator->interpolation_node_no);
	printf("--------------------------------\n\n");
	flushall();
}

void newsmooth::create_velocity_vectors(int axes_num) {
	joint_velocity = vector<double>(axes_num, 0.15);
	joint_max_velocity = vector<double>(axes_num, 1.5);
	joint_acceleration = vector<double>(axes_num, 0.02);
	joint_max_acceleration = vector<double>(axes_num, 7.0);
	motor_velocity = vector<double>(axes_num, 0.15);
	motor_max_velocity = vector<double>(axes_num, 200.0);
	motor_acceleration = vector<double>(axes_num, 0.02);
	motor_max_acceleration = vector<double>(axes_num, 150.0);
	euler_zyz_velocity= vector<double>(axes_num, 0.15);//TODO check if this is a reasonable value
	euler_zyz_max_velocity = vector<double>(axes_num, 5.0);
	euler_zyz_acceleration = vector<double>(axes_num, 0.02);//TODO check if this is a reasonable value
	euler_zyz_max_acceleration = vector<double>(axes_num, 5.0);
	angle_axis_velocity = vector<double>(axes_num, 0.15);//TODO check if this is a reasonable value
	angle_axis_max_velocity = vector<double>(axes_num, 5.0);
	angle_axis_acceleration = vector<double>(axes_num, 0.02);//TODO check if this is a reasonable value
	angle_axis_max_acceleration = vector<double>(axes_num, 5.0);
}

//--------------- METHODS USED TO LOAD POSES ----------------

bool newsmooth::load_absolute_joint_trajectory_pose(const vector<double> & coordinates) {

	ecp_mp::common::trajectory_pose::bang_bang_trajectory_pose pose;

	return load_trajectory_pose(coordinates, lib::ABSOLUTE, lib::ECP_JOINT, joint_velocity, joint_acceleration, joint_max_velocity, joint_max_acceleration);
}

bool newsmooth::load_relative_joint_trajectory_pose(const vector<double> & coordinates) {

	ecp_mp::common::trajectory_pose::bang_bang_trajectory_pose pose;

	return load_trajectory_pose(coordinates, lib::RELATIVE, lib::ECP_JOINT, joint_velocity, joint_acceleration, joint_max_velocity, joint_max_acceleration);
}

bool newsmooth::load_absolute_motor_trajectory_pose(const vector<double> & coordinates) {

	ecp_mp::common::trajectory_pose::bang_bang_trajectory_pose pose;

	return load_trajectory_pose(coordinates, lib::ABSOLUTE, lib::ECP_MOTOR, motor_velocity, motor_acceleration, motor_max_velocity, motor_max_acceleration);
}

bool newsmooth::load_relative_motor_trajectory_pose(const vector<double> & coordinates) {

	ecp_mp::common::trajectory_pose::bang_bang_trajectory_pose pose;

	return load_trajectory_pose(coordinates, lib::RELATIVE, lib::ECP_MOTOR, motor_velocity, motor_acceleration, motor_max_velocity, motor_max_acceleration);
}

bool newsmooth::load_absolute_euler_zyz_trajectory_pose(const vector<double> & coordinates) {

	ecp_mp::common::trajectory_pose::bang_bang_trajectory_pose pose;

	return load_trajectory_pose(coordinates, lib::ABSOLUTE, lib::ECP_XYZ_EULER_ZYZ, euler_zyz_velocity, euler_zyz_acceleration, euler_zyz_max_velocity, euler_zyz_max_acceleration);
}

bool newsmooth::load_relative_euler_zyz_trajectory_pose(const vector<double> & coordinates) {

	ecp_mp::common::trajectory_pose::bang_bang_trajectory_pose pose;

	return load_trajectory_pose(coordinates, lib::RELATIVE, lib::ECP_XYZ_EULER_ZYZ, euler_zyz_velocity, euler_zyz_acceleration, euler_zyz_max_velocity, euler_zyz_max_acceleration);
}

bool newsmooth::load_absolute_angle_axis_trajectory_pose(const vector<double> & coordinates) {

	ecp_mp::common::trajectory_pose::bang_bang_trajectory_pose pose;

	return load_trajectory_pose(coordinates, lib::ABSOLUTE, lib::ECP_XYZ_ANGLE_AXIS, angle_axis_velocity, angle_axis_acceleration, angle_axis_max_velocity, angle_axis_max_acceleration);
}

bool newsmooth::load_relative_angle_axis_trajectory_pose(const vector<double> & coordinates) {

	ecp_mp::common::trajectory_pose::bang_bang_trajectory_pose pose;

	return load_trajectory_pose(coordinates, lib::RELATIVE, lib::ECP_XYZ_ANGLE_AXIS, angle_axis_velocity, angle_axis_acceleration, angle_axis_max_velocity, angle_axis_max_acceleration);
}

bool newsmooth::load_trajectory_pose(const vector<double> & coordinates, lib::MOTION_TYPE motion_type, lib::ECP_POSE_SPECIFICATION pose_spec, const vector<double> & v, const vector<double> & a, const vector<double> & v_max, const vector<double> & a_max) {

	if (!pose_vector.empty() && this->pose_spec != pose_spec) { //check if previous positions were provided in joint representation

		sr_ecp_msg.message("Representation different than the previous one");
		return false;
	}

	if (!pose_vector.empty() && this->motion_type != motion_type) {

		sr_ecp_msg.message("Wrong motion type");
		return false;
	}

	this->motion_type = motion_type;
	this->pose_spec = pose_spec;

	ecp_mp::common::trajectory_pose::bang_bang_trajectory_pose pose; //new trajectory pose
	pose = ecp_mp::common::trajectory_pose::bang_bang_trajectory_pose(pose_spec, coordinates, v, a); //create new trajectory pose
	pose.v_max = v_max; //set the v_max vector
	pose.a_max = a_max; //set the a_max vector

	if (pose_vector.empty()) {
		pose.pos_num = 1;
	} else {
		pose.pos_num = pose_vector.back().pos_num + 1;
	}

	if (motion_type == lib::ABSOLUTE) {
		if (!pose_vector.empty()) {//set the start position of the added pose as the desired position of the previous pose
			pose.start_position = pose_vector.back().coordinates;
		}
	}

	pose_vector.push_back(pose); //put new trajectory pose into a pose vector

	sr_ecp_msg.message("Pose loaded");

	return true;
}

//--------------- METHODS USED TO LOAD POSES END ----------------

} // namespace generator
} // namespace common
} // namespace ecp
} // namespace mrrocpp
