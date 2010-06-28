// ------------------------------------------------------------------------
// Proces:		EDP
// Plik:			kinematic_model_irp6p_jacobian_transpose_with_wrist.h
// System:	QNX/MRROC++  v. 6.3
// Opis:		Model kinematyki robota IRp-6 na postumencie
//				- deklaracja klasy
//				- przedefiniowanie rozwiazania odwrotnego zadania
//				  kinematyki - metoda uwzgledniajaca jakobian transponowany
//
// Autor:		Anna Maria Sibilska
// Data:		18.07.2007
// ------------------------------------------------------------------------

#if !defined(_IRP6P_KIN_MODEL_WITH_WRIST_JACOBIAN_TRANSPOSE)
#define _IRP6P_KIN_MODEL_WITH_WRIST_JACOBIAN_TRANSPOSE

// Definicja klasy kinematic_model.
#include "robot/irp6p_m/kinematic_model_irp6p_with_wrist.h"

namespace mrrocpp {
namespace kinematics {
namespace irp6p {

class model_jacobian_transpose_with_wrist: public model_with_wrist
{

public:
  // Konstruktor.
  model_jacobian_transpose_with_wrist (int _number_of_servos);

  //Rozwiazanie odwrotnego zagadnienia kinematyki.
  virtual void inverse_kinematics_transform(lib::JointArray & local_desired_joints, lib::JointArray & local_current_joints, const lib::Homog_matrix& local_desired_end_effector_frame);

};//: kinematic_model_irp6p_jacobian_transpose_with_wrist

} // namespace irp6p
} // namespace kinematic
} // namespace mrrocpp

#endif
