/*!
 * @file kinematic_model_irp6p_tfg.h
 * @brief File containing the IRp-6p two fingered gripper kinematic model class.
 *
 * @author yoyek
 * @author tkornuta
 * @date Jun 21, 2010
 *
 * @ingroup KINEMATICS IRP6P_KINEMATICS
 */


#if !defined(_IRP6P_TFG_KIN_MODEL)
#define _IRP6P_TFG_KIN_MODEL

#include "robot/irp6_tfg/kinematic_model_irp6_tfg.h"

namespace mrrocpp {
namespace kinematics {
namespace irp6p_tfg {

/*!
 * @class model
 * @brief Kinematic model for two fingered gripper of the IRp-6 on postument manipulator.
 *
 * @author tkornuta
 * @date Jun 21, 2010
 *
 * @ingroup KINEMATICS IRP6P_KINEMATICS
 */
class model: public common::kinematic_model_irp6_tfg
{
protected:

	//! Method responsible for kinematic parameters setting.
	void set_kinematic_parameters(void);

public:
	//! Constructor.
	model(void);

};

} // namespace irp6p_tfg
} // namespace kinematic
} // namespace mrrocpp

#endif
