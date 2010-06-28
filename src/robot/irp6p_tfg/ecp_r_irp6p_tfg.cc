// -------------------------------------------------------------------------
//                            ecp.cc
//            Effector Control Process (lib::ECP) - methods
// Funkcje do tworzenia procesow ECP
// robot - irp6_postument
//
// -------------------------------------------------------------------------

#include "lib/impconst.h"
#include "lib/com_buf.h"
#include "lib/mis_fun.h"

#include "robot/irp6p_tfg/ecp_r_irp6p_tfg.h"

namespace mrrocpp {
namespace ecp {
namespace irp6p_tfg {

robot::robot(lib::configurator &_config, lib::sr_ecp &_sr_ecp) :
	ecp_robot(lib::ROBOT_IRP6P_TFG, IRP6P_TFG_NUM_OF_SERVOS, EDP_IRP6P_TFG_SECTION, _config, _sr_ecp),
	kinematics_manager()
{
	//  Stworzenie listy dostepnych kinematyk.
	create_kinematic_models_for_given_robot();
}

robot::robot(common::task::task& _ecp_object) :
	ecp_robot(lib::ROBOT_IRP6P_TFG, IRP6P_TFG_NUM_OF_SERVOS, EDP_IRP6P_TFG_SECTION, _ecp_object),
	kinematics_manager()
{
	//  Stworzenie listy dostepnych kinematyk.
	create_kinematic_models_for_given_robot();
}

// Stworzenie modeli kinematyki dla robota IRp-6 na postumencie.
void robot::create_kinematic_models_for_given_robot(void)
{
	// Stworzenie wszystkich modeli kinematyki.
	add_kinematic_model(new kinematics::irp6p_tfg::model());
	// Ustawienie aktywnego modelu.
	set_kinematic_model(0);
}

} // namespace irp6p
} // namespace ecp
} // namespace mrrocpp
