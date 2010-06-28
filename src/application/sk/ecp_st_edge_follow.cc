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
#include "application/sk/ecp_g_sk.h"
#include "application/sk/ecp_st_edge_follow.h"

namespace mrrocpp {
namespace ecp {
namespace common {
namespace task {

ecp_sub_task_edge_follow::ecp_sub_task_edge_follow(task &_ecp_t) :
	ecp_sub_task(_ecp_t)
{
	yefg = new generator::y_edge_follow_force(_ecp_t, 8);
}

void ecp_sub_task_edge_follow::conditional_execution()
{

	yefg->Move();
}

} // namespace task

} // namespace common
} // namespace ecp
} // namespace mrrocpp
