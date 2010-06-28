// -------------------------------------------------------------------------
//                            ecp_st_go.cc
//            Effector Control Process (lib::ECP) - methods
// Funkcje do tworzenia procesow ECP
//
// Ostatnia modyfikacja: 2007
// -------------------------------------------------------------------------

#include "lib/typedefs.h"
#include "lib/impconst.h"
#include "lib/com_buf.h"

#include "lib/srlib.h"
#include "generator/ecp/ecp_g_jarosz.h"
#include "subtask/ecp_st_go.h"

namespace mrrocpp {
namespace ecp {
namespace common {
namespace task {

ecp_sub_task_gripper_opening::ecp_sub_task_gripper_opening(task &_ecp_t) :
	ecp_sub_task(_ecp_t)
{
	init();
}

void ecp_sub_task_gripper_opening::init()
{
	/*
	 tdes.arm_type = lib::XYZ_EULER_ZYZ;
	 tdes.interpolation_node_no = 1;
	 // Wspolrzedne kartezjanskie XYZ i katy Eulera ZYZ
	 tdes.coordinate_delta[0] = 0.0; // przyrost wspolrzednej X
	 tdes.coordinate_delta[1] = 0.0;// przyrost wspolrzednej Y
	 tdes.coordinate_delta[2] = 0.0; // przyrost wspolrzednej Z
	 tdes.coordinate_delta[3] = 0.0; // przyrost wspolrzednej FI
	 tdes.coordinate_delta[4] = 0.0; // przyrost wspolrzednej TETA
	 tdes.coordinate_delta[5] = 0.0; // przyrost wspolrzednej PSI
	 //	tdes.coordinate_delta[6] = 0.0;   // przyrost wspolrzednej PSI
	 */
}

void ecp_sub_task_gripper_opening::configure(double gripper_increment, int motion_time)
{
	tdes.internode_step_no = motion_time;
	tdes.value_in_step_no = tdes.internode_step_no - 2;
	tdes.coordinate_delta[6] = gripper_increment; // przyrost wspolrzednej PSI
}

void ecp_sub_task_gripper_opening::execute()
{
	// Generator trajektorii prostoliniowej
	generator::linear lg(ecp_t, tdes, 1);
	lg.Move();
}

void ecp_sub_task_gripper_opening::conditional_execution()
{

}

} // namespace task

} // namespace common
} // namespace ecp
} // namespace mrrocpp
