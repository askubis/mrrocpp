// -------------------------------------------------------------------------
//                            ui_class.h
// Definicje klasy Ui
//
// Ostatnia modyfikacja: 2010
// -------------------------------------------------------------------------

#ifndef __UI_R_SARKOFAG_H
#define __UI_R_SARKOFAG_H

#include "ui/src/ui.h"
#include "ui/src/ui_robot.h"
#include "robot/sarkofag/sarkofag_const.h"

//
//
// KLASA UiRobotSarkofag
//
//


// super klasa agregujaca porozrzucane struktury

class Ui;
class ui_tfg_and_conv_robot;

class UiRobotSarkofag : public UiRobot
{
private:

public:

	double sarkofag_current_pos[SARKOFAG_NUM_OF_SERVOS];// pozycja biezaca
	double sarkofag_desired_pos[SARKOFAG_NUM_OF_SERVOS]; // pozycja zadana


	bool is_wind_sarkofag_moves_open; // informacja czy okno ruchow
	bool is_wind_sarkofag_servo_algorithm_open; // informacja czy okno definicji kinematyki jest otwarte

	ui_tfg_and_conv_robot *ui_ecp_robot;

	UiRobotSarkofag(Ui& _ui);
	int reload_configuration();
	int manage_interface();
	int close_all_windows();
	int delete_ui_ecp_robot();
};

#endif

