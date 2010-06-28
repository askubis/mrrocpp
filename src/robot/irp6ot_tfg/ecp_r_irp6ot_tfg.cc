// -------------------------------------------------------------------------
//                            ecp.cc
//            Effector Control Process (lib::ECP) - methods
// Funkcje do tworzenia procesow ECP
// robot - irp6_on_track
//
// -------------------------------------------------------------------------

#include "lib/impconst.h"
#include "lib/com_buf.h"
#include "lib/mis_fun.h"

#include "robot/irp6ot_tfg/ecp_r_irp6ot_tfg.h"

namespace mrrocpp {
namespace ecp {
namespace irp6ot_tfg {

robot::robot(lib::configurator &_config, lib::sr_ecp &_sr_ecp) :
	ecp_robot(lib::ROBOT_IRP6OT_TFG, IRP6OT_TFG_NUM_OF_SERVOS, EDP_IRP6OT_TFG_SECTION, _config, _sr_ecp),
			kinematics_manager()
{
	//  Stworzenie listy dostepnych kinematyk.
	create_kinematic_models_for_given_robot();
}

robot::robot(common::task::task& _ecp_object) :
	ecp_robot(lib::ROBOT_IRP6OT_TFG, IRP6OT_TFG_NUM_OF_SERVOS, EDP_IRP6OT_TFG_SECTION, _ecp_object),
			kinematics_manager()
{
	//  Stworzenie listy dostepnych kinematyk.
	create_kinematic_models_for_given_robot();
}

// Stworzenie modeli kinematyki dla robota IRp-6 na torze.
void robot::create_kinematic_models_for_given_robot(void)
{
	// Stworzenie wszystkich modeli kinematyki.
	add_kinematic_model(new kinematics::irp6ot_tfg::model());
	// Ustawienie aktywnego modelu.
	set_kinematic_model(0);

}

} // namespace irp6ot
} // namespace ecp
} // namespace mrrocpp


