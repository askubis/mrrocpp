/*
 * velocity_profile.h
 *
 *  Created on: May 4, 2010
 *      Author: rtulwin
 */

#ifndef _VELOCITY_PROFILE_H_
#define _VELOCITY_PROFILE_H_

//#include <list>
#include <math.h>
#include <algorithm>
//#include <vector>

#include "lib/trajectory_pose/trajectory_pose.h"

using namespace std;

namespace mrrocpp {
namespace ecp {
namespace common {
namespace generator {
namespace velocity_profile_calculator {

/**
 * Base class for all of the velocity profile calculators. Usually any velocity profile calculator contains methods used to create the description
 * of the velocity profile f.g. bang bang velocity profile etc.. This information is usually stored in the appropriate trajectory_pose class.
 */
class velocity_profile {
	public:
		/**
		 * Constructor.
		 */
		velocity_profile();
		/**
		 * Destructor.
		 */
		virtual ~velocity_profile();
		/**
		 * Method comparing two double values.
		 * @return true if values are the same
		 */
		bool eq(double a, double b);
};

} // namespace velocity_profile_calculator
} // namespace generator
} // namespace common
} // namespace ecp
} // namespace mrrocpp

#endif /* _VELOCITY_PROFILE_H_ */
