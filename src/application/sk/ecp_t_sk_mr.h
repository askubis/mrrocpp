#if !defined(_ECP_T_SK_MR_H)
#define _ECP_T_SK_MR_H

#include "base/ecp/ecp_task.h"

namespace mrrocpp {
namespace ecp {
namespace common {
namespace task {

class sk_mr : public common::task::task
{
protected:

public:
	// KONSTRUKTORY
	sk_mr(lib::configurator &_config);

};

}
} // namespace common
} // namespace ecp
} // namespace mrrocpp

#endif
