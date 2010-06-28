#if !defined(MP_R_POLYCRANK_H_)
#define MP_R_POLYCRANK_H_

#include "base/mp/mp_r_motor_driven.h"

namespace mrrocpp {
namespace mp {
namespace robot {

class polycrank : public motor_driven
{
public:
	polycrank(task::task &mp_object_l);
};

} // namespace robot
} // namespace mp
} // namespace mrrocpp
#endif /*MP_R_POLYCRANK_H_*/
