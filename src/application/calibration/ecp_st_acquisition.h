#if !defined(_ECP_ST_ACQUISITION_H)
#define _ECP_ST_ACQUISITION_H

#include "lib/impconst.h"
#include "lib/com_buf.h"
#include "base/ecp/ecp_task.h"

namespace mrrocpp {
namespace ecp {
namespace common {
namespace task {

class acquisition: public ecp_sub_task {
	public:
		// KONSTRUKTORY
		acquisition(task &_ecp_t);

		// methods for ECP template to redefine in concrete classes
		void main_task_algorithm(void);

		virtual void write_data(const std::string & _K_fp, const std::string & _kk_fp, const std::string & _M_fp, const std::string & _mm_fp, int _number_of_measures) = 0;
};

} // namespace task
} // namespace common
} // namespace ecp
} // namespace mrrocpp

#endif
