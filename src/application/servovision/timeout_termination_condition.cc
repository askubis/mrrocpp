/*
 * timeout_termination_condition.cc
 *
 *  Created on: Jun 2, 2010
 *      Author: mboryn
 */

#include "timeout_termination_condition.h"

namespace mrrocpp {
namespace ecp {
namespace servovision {

timeout_termination_condition::timeout_termination_condition(double timeout) :
	timeout(timeout), time_left(timeout)
{
}

timeout_termination_condition::~timeout_termination_condition()
{
}

void timeout_termination_condition::reset()
{
	time_left = timeout;
}

void timeout_termination_condition::update(const visual_servo_manager* vsm)
{

}

bool timeout_termination_condition::is_condition_met() const
{
	return time_left <= 0;
}

}//namespace generator
}
}
