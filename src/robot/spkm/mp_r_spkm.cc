#include "robot/spkm/mp_r_spkm.h"
#include "robot/spkm/spkm_const.h"

namespace mrrocpp {
namespace mp {
namespace robot {

spkm::spkm(task::task &mp_object_l) :
	motor_driven(lib::ROBOT_SPKM, ECP_SPKM_SECTION, mp_object_l, SPKM_NUM_OF_SERVOS)
{
}

} // namespace robot
} // namespace mp
} // namespace mrrocpp
