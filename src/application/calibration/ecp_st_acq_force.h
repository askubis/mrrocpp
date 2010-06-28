
#if !defined(_ECP_ST_KCZ_FORCE_H)
#define _ECP_ST_KCZ_FORCE_H

#include "base/ecp/ecp_task.h"
#include "ecp_g_pcbird_nose_run.h"
#include "ecp_st_acquisition.h"

namespace mrrocpp {
namespace ecp {
namespace common {
namespace task {

class acq_force: public acquisition {

  protected:
	  std::string K_fp;
	  std::string kk_fp;
	  std::string M_fp;
	  std::string mm_fp;
	  int number_of_measures;

	  void main_task_algorithm(void);

  public:
	  common::generator::pcbird_nose_run* nose_run;
	  acq_force(task &_ecp_t);
	  void write_data(std::string _K_fp, std::string _kk_fp, std::string _M_fp, std::string _mm_fp, int _number_of_measures);
	  void conditional_execution();

};

}
} // namespace common
} // namespace ecp
} // namespace mrrocpp

#endif

