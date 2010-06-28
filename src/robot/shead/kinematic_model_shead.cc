#include "lib/com_buf.h"

// Klasa kinematic_model_irp6m_with_wrist.
#include "robot/shead/kinematic_model_shead.h"

namespace mrrocpp {
namespace kinematics {
namespace shead {

model::model(void)
{
	// Ustawienie etykiety modelu kinematycznego.
	set_kinematic_model_label("Switching to simple kinematic model");

	// Ustawienie parametrow kinematycznych.
	set_kinematic_parameters();
}

void model::set_kinematic_parameters(void)
{
}

void model::check_motor_position(const lib::MotorArray & motor_position)
{
}

void model::check_joints(const lib::JointArray & q)
{
}

void model::mp2i_transform(const lib::MotorArray & local_current_motor_pos, lib::JointArray & local_current_joints)
{
}

void model::i2mp_transform(lib::MotorArray & local_desired_motor_pos_new, lib::JointArray & local_desired_joints)
{
}

} // namespace smb
} // namespace kinematic
} // namespace mrrocpp

