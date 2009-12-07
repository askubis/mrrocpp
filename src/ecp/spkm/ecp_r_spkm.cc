// -------------------------------------------------------------------------
//                            ecp.cc
//            Effector Control Process (lib::ECP) - methods
// Funkcje do tworzenia procesow ECP
// robot - irp6_mechatronika
//
// -------------------------------------------------------------------------

#include "lib/impconst.h"
#include "lib/com_buf.h"
#include "lib/mis_fun.h"

#include "ecp/spkm/ecp_r_spkm.h"

namespace mrrocpp {
namespace ecp {
namespace spkm {

robot::robot (lib::configurator &_config, lib::sr_ecp &_sr_ecp):
	ecp_robot (lib::ROBOT_SPKM, SPKM_NUM_OF_SERVOS, EDP_SPKM_SECTION, _config, _sr_ecp){};
robot::robot (common::task::task& _ecp_object):
	ecp_robot (lib::ROBOT_SPKM, SPKM_NUM_OF_SERVOS, EDP_SPKM_SECTION, _ecp_object){};

// --------------------------------------------------------------------------
void robot::create_command (void) {
  // wypelnia bufor wysylkowy do EDP na podstawie danych
  // zawartych w skladowych generatora lub warunku

  ecp_command.instruction.instruction_type = ecp_command.instruction.instruction_type;
  ecp_command.instruction.set_type = ecp_command.instruction.set_type;
  ecp_command.instruction.get_type = ecp_command.instruction.get_type;
// printf("ecp_command.instruction.get_type: %d, ecp_command.instruction.get_type: %d\n",
// ecp_command.instruction.get_type,ecp_command.instruction.get_type);

  ecp_command.instruction.set_rmodel_type = ecp_command.instruction.set_rmodel_type;
  ecp_command.instruction.get_rmodel_type = ecp_command.instruction.get_rmodel_type;
  ecp_command.instruction.set_arm_type = ecp_command.instruction.set_arm_type;
  ecp_command.instruction.get_arm_type = ecp_command.instruction.get_arm_type;
  ecp_command.instruction.output_values = ecp_command.instruction.output_values;

  switch (ecp_command.instruction.instruction_type) {
    case lib::SET:
    case lib::SET_GET:

        if (ecp_command.instruction.set_type & RMODEL_DV) {
          switch (ecp_command.instruction.set_rmodel_type) {
            case lib::TOOL_FRAME:
            lib::copy_frame(ecp_command.instruction.rmodel.tool_frame_def.tool_frame, ecp_command.instruction.rmodel.tool_frame_def.tool_frame);
              break;
            case lib::TOOL_XYZ_ANGLE_AXIS:
              for (int j=0; j<6; j++)
                ecp_command.instruction.rmodel.tool_coordinate_def.tool_coordinates[j]
                     = ecp_command.instruction.rmodel.tool_coordinate_def.tool_coordinates[j];
              break;
            case lib::TOOL_XYZ_EULER_ZYZ:
              for (int j=0; j<6; j++)
                ecp_command.instruction.rmodel.tool_coordinate_def.tool_coordinates[j]
                     = ecp_command.instruction.rmodel.tool_coordinate_def.tool_coordinates[j];
              break;
			  ///////////////////K
			case lib::TOOL_AS_XYZ_EULER_ZY:
              for (int j=0; j<6; j++)
                ecp_command.instruction.rmodel.tool_coordinate_def.tool_coordinates[j]
                     = ecp_command.instruction.rmodel.tool_coordinate_def.tool_coordinates[j];
              break;
			  /////////////////K
            case lib::ARM_KINEMATIC_MODEL:
                ecp_command.instruction.rmodel.kinematic_model.kinematic_model_no
                    = ecp_command.instruction.rmodel.kinematic_model.kinematic_model_no;
              break;
            case lib::SERVO_ALGORITHM:
              for (int j=0; j<SPKM_NUM_OF_SERVOS; j++) {
                ecp_command.instruction.rmodel.servo_algorithm.servo_algorithm_no[j]
                   = ecp_command.instruction.rmodel.servo_algorithm.servo_algorithm_no[j];
                ecp_command.instruction.rmodel.servo_algorithm.servo_parameters_no[j]
                   = ecp_command.instruction.rmodel.servo_algorithm.servo_parameters_no[j];
              }; // end: for
              break;
            default: // Blad: niewlasciwy typ modelu robota
              throw ECP_error(lib::NON_FATAL_ERROR, INVALID_RMODEL_TYPE);
          } // end: switch (set_rmodel_type)
        }

         if (ecp_command.instruction.set_type & ARM_DV) {
          ecp_command.instruction.motion_type = ecp_command.instruction.motion_type;
          ecp_command.instruction.interpolation_type = ecp_command.instruction.interpolation_type;
          ecp_command.instruction.motion_steps = ecp_command.instruction.motion_steps;
          ecp_command.instruction.value_in_step_no = ecp_command.instruction.value_in_step_no;
          // Wypelniamy czesc zwiazana z polozeniem ramienia
          switch (ecp_command.instruction.set_arm_type) {
            case lib::FRAME:
            lib::copy_frame(ecp_command.instruction.arm.pf_def.arm_frame, ecp_command.instruction.arm.pf_def.arm_frame);
               break;
            case lib::XYZ_ANGLE_AXIS:
              for (int j=0; j<6 ; j++)
                ecp_command.instruction.arm.pf_def.arm_coordinates[j]
                   = ecp_command.instruction.arm.pf_def.arm_coordinates[j];
             break;
            case lib::XYZ_EULER_ZYZ:
              for (int j=0; j<6 ; j++)
                ecp_command.instruction.arm.pf_def.arm_coordinates[j]
                   = ecp_command.instruction.arm.pf_def.arm_coordinates[j];
              break;
            case lib::JOINT:
              for (int j=0; j<SPKM_NUM_OF_SERVOS ; j++){
                ecp_command.instruction.arm.pf_def.arm_coordinates[j]
                   = ecp_command.instruction.arm.pf_def.arm_coordinates[j];
                  }
              break;
            case lib::MOTOR:
              for (int j=0; j<SPKM_NUM_OF_SERVOS ; j++)
                ecp_command.instruction.arm.pf_def.arm_coordinates[j]
                   = ecp_command.instruction.arm.pf_def.arm_coordinates[j];
              break;
            default: // Blad: niewlasciwy sposob zadawania polozenia ramienia
              throw ECP_error (lib::NON_FATAL_ERROR, INVALID_POSE_SPECIFICATION);
          } // end: (set_arm_type)

        }
      break;
    case lib::GET:
    case lib::SYNCHRO:
    case lib::QUERY:
      break;
    default: // blad: nieprawidlowe polecenie
      throw ECP_error (lib::NON_FATAL_ERROR, INVALID_ECP_COMMAND);
  } // end: switch (instruction_type)

}; // end: robot::create_command
// ---------------------------------------------------------------


/*---------------------------------------------------------------------*/
void robot::get_reply (void) {
  // pobiera z pakietu przeslanego z EDP informacje i wstawia je do
  // odpowiednich skladowych generatora lub warunku

 reply_package.reply_type = reply_package.reply_type;

 switch (reply_package.reply_type) {
   case lib::ERROR:
     reply_package.error_no.error0 = reply_package.error_no.error0;
     reply_package.error_no.error1 = reply_package.error_no.error1;
     break;
   case lib::ACKNOWLEDGE:
     break;
   case lib::SYNCHRO_OK:
     break;
   case lib::ARM_INPUTS:
	get_input_reply();
   case lib::ARM:
	get_arm_reply();
     break;
  case lib::RMODEL_INPUTS:
	get_input_reply();
   case lib::RMODEL:
   	get_rmodel_reply();
     break;
   case lib::INPUTS:
	get_input_reply();
     break;
   case lib::ARM_RMODEL_INPUTS:
   	get_input_reply();
   case lib::ARM_RMODEL:
	get_arm_reply();
	get_rmodel_reply();
     break;
   default:  // bledna przesylka
     throw ECP_error (lib::NON_FATAL_ERROR, INVALID_EDP_REPLY);
 }
}



void robot::get_input_reply (void)
{
    reply_package.input_values = reply_package.input_values;
	for (int i=0; i<8; i++) {
		reply_package.analog_input[i]=reply_package.analog_input[i];
	}
}


void robot::get_arm_reply (void)
{
    switch (reply_package.arm_type) {
       case lib::MOTOR:
         for (int i=0; i<SPKM_NUM_OF_SERVOS; i++)
           reply_package.arm.pf_def.arm_coordinates[i] =
             reply_package.arm.pf_def.arm_coordinates[i];
         break;
       case lib::JOINT:
         for (int i=0; i<SPKM_NUM_OF_SERVOS; i++)
           reply_package.arm.pf_def.arm_coordinates[i] =
             reply_package.arm.pf_def.arm_coordinates[i];
               break;
       case lib::FRAME:
       lib::copy_frame(reply_package.arm.pf_def.arm_frame, reply_package.arm.pf_def.arm_frame);
        break;
       case lib::XYZ_EULER_ZYZ:
        for (int i=0; i<6; i++)
           reply_package.arm.pf_def.arm_coordinates[i] =
             reply_package.arm.pf_def.arm_coordinates[i];
         break;

       case lib::XYZ_ANGLE_AXIS:
         for (int i=0; i<6; i++)
           reply_package.arm.pf_def.arm_coordinates[i] =
             reply_package.arm.pf_def.arm_coordinates[i];
         break;

       default: // bledny typ specyfikacji pozycji
         throw ECP_error (lib::NON_FATAL_ERROR, INVALID_POSE_SPECIFICATION);
     } // end: switch (...arm_type)
}

void robot::get_rmodel_reply (void)
{
	switch (reply_package.rmodel_type) {
		case lib::TOOL_FRAME:
			lib::copy_frame(reply_package.rmodel.tool_frame_def.tool_frame, reply_package.rmodel.tool_frame_def.tool_frame);
			break;
		case lib::TOOL_XYZ_ANGLE_AXIS:
			for (int i=0; i<6; i++)
				reply_package.rmodel.tool_coordinate_def.tool_coordinates[i] =
				    reply_package.rmodel.tool_coordinate_def.tool_coordinates[i];
			break;
		case lib::TOOL_XYZ_EULER_ZYZ:
			for (int i=0; i<6; i++)
				reply_package.rmodel.tool_coordinate_def.tool_coordinates[i] =
				    reply_package.rmodel.tool_coordinate_def.tool_coordinates[i];
			break;
			////////////////////K
		case lib::TOOL_AS_XYZ_EULER_ZY:
			for (int i=0; i<6; i++)
				reply_package.rmodel.tool_coordinate_def.tool_coordinates[i] =
				    reply_package.rmodel.tool_coordinate_def.tool_coordinates[i];
			break;
			//////////////////K
		case lib::ARM_KINEMATIC_MODEL:
			reply_package.rmodel.kinematic_model.kinematic_model_no =
			    reply_package.rmodel.kinematic_model.kinematic_model_no;
			break;
		case lib::SERVO_ALGORITHM:
			for (int i=0; i<SPKM_NUM_OF_SERVOS; i++) {
				reply_package.rmodel.servo_algorithm.servo_algorithm_no[i] =
				    reply_package.rmodel.servo_algorithm.servo_algorithm_no[i];
				reply_package.rmodel.servo_algorithm.servo_parameters_no[i] =
				    reply_package.rmodel.servo_algorithm.servo_parameters_no[i];
			}
			break;
		default: // bledny typ specyfikacji modelu robota
			throw ECP_error(lib::NON_FATAL_ERROR, INVALID_POSE_SPECIFICATION);
	} // end: switch (...rmodel_type)
}

} // namespace spkm
} // namespace ecp
} // namespace mrrocpp
