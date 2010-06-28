// ------------------------------------------------------------------------
// Proces:		EDP
// Plik:			kinematic_model_conveyor.h
// System:	QNX/MRROC++  v. 6.3
// Opis:		Model kinematyki tasmociogu
//				- deklaracja klasy
//
// Autor:		tkornuta
// Data:		31.01.2007
// ------------------------------------------------------------------------

#if !defined(_CONVEYOR_KIN_MODEL)
#define _CONVEYOR_KIN_MODEL

#include "base/kinematics/kinematic_model.h"

namespace mrrocpp {
namespace kinematics {
namespace conveyor {

class model : public common::kinematic_model
{
protected:
  // Polozenie synchronizacji.
  double synchro_motor_position;
  // Stosunek polozenia walu silnika do polozenia we wsp. wewn (zewn) w metrach.
  double motor_to_intext_ratio;

  // Ustawienie parametrow kinematycznych.
  virtual void set_kinematic_parameters(void);

  // Sprawdzenie ograniczen na polozenia katowe walow silnikow
  virtual void check_motor_position(const lib::MotorArray & motor_position);
  // Sprawdzenie ograniczen na wspolrzedne wewnetrzne
  virtual void check_joints(const lib::JointArray & q);

public:
  // Konstruktor.
  model (void);

  // Przeliczenie polozenia walow silnikow na wspolrzedne wewnetrzne.
  virtual void mp2i_transform(const lib::MotorArray & local_current_motor_pos, lib::JointArray & local_current_joints);

  // Przeliczenie wspolrzednych wewnetrznych na polozenia walow silnikow.
  virtual void i2mp_transform(lib::MotorArray & local_desired_motor_pos_new, lib::JointArray & local_desired_joints);


};//: kinematic_model_conveyor;

} // namespace conveyor
} // namespace kinematic
} // namespace mrrocpp

#endif
