// -------------------------------------------------------------------------
//                            hi_rydz.h
// Definicje struktur danych i metod dla interfejsu sprzetowego
//
// Ostatnia modyfikacja: 16.04.98
// -------------------------------------------------------------------------

#ifndef __HI_LOCAL_IRP6OT_H
#define __HI_LOCAL_IRP6OT_H

#include "edp/common/hi_rydz.h"

namespace mrrocpp {
namespace edp {
namespace irp6ot {

// Struktury danych wykorzystywane w hardware_interface
const int IRQ_REAL = 9; // Numer przerwania sprzetowego
const unsigned short int INT_FREC_DIVIDER = 4; // Dzielnik czestotliwosci przerwan

#define HI_RYDZ_INTR_TIMEOUT_HIGH 10000000 // by Y - timeout przerwania z szafy badz zegara

#define FIRST_SERVO_PTR           0xC0
#define INTERRUPT_GENERATOR_SERVO_PTR	 0xC0
#define IN_OUT_PACKET		0xC8

#define ISA_CARD_OFFSET 0x20 // w zaleznosci od ustawienia na karcie isa

#define IRP6_ON_TRACK_AXIS_1_MAX_CURRENT           0x2430 // ustawienie pradu maksymalnego dla przedostatniej osi - obrot chwytaka
#define IRP6_ON_TRACK_AXIS_2_MAX_CURRENT           0x2430 // ustawienie pradu maksymalnego dla przedostatniej osi - obrot chwytaka
#define IRP6_ON_TRACK_AXIS_3_MAX_CURRENT           0x2430 // ustawienie pradu maksymalnego dla przedostatniej osi - obrot chwytaka
#define IRP6_ON_TRACK_AXIS_4_MAX_CURRENT           0x2430 // ustawienie pradu maksymalnego dla przedostatniej osi - obrot chwytaka
#define IRP6_ON_TRACK_AXIS_5_MAX_CURRENT           0x2430 // ustawienie pradu maksymalnego dla przedostatniej osi - obrot chwytaka
#define IRP6_ON_TRACK_AXIS_6_MAX_CURRENT           0x2430 // ustawienie pradu maksymalnego dla przedostatniej osi - obrot chwytaka
#define IRP6_ON_TRACK_AXIS_7_MAX_CURRENT           0x2410 // ustawienie pradu maksymalnego dla przedostatniej osi - obrot chwytaka
// 13,7 j na amper

#define IRP6_ON_TRACK_AXIS_8_MAX_CURRENT           0x2427 // ustawienie pradu maksymalnego dla zacisku chwytaka
// by Y - UWAGA nieczulosc nieznana, rozdzielczosc do ustalenia
// 25,3 j na 100ma, strefa nieczulosci na poziomie 40ma

#define IRP6_ON_TRACK_AXIS_0_TO_5_INC_PER_REVOLUTION   682.0  // Liczba impulsow rezolwera na obrot walu - musi byc float
#define IRP6_ON_TRACK_AXIS_6_INC_PER_REVOLUTION  2000.0  // Liczba impulsow enkodera na obrot walu - musi byc float
#define IRP6_ON_TRACK_AXIS_7_INC_PER_REVOLUTION  128.0  // Liczba impulsow enkodera na obrot walu - musi byc float

// ------------------------------------------------------------------------
//                HARDWARE_INTERFACE class
// ------------------------------------------------------------------------

class hardware_interface: public common::hardware_interface {

public:
	hardware_interface(effector &_master); // Konstruktor
	~hardware_interface(void); // Destruktor
	bool is_hardware_error(void); // Sprawdzenie czy wystapil blad sprzetowy

	uint64_t read_write_hardware(void); // Obsluga sprzetu
	void reset_counters(void); // Zerowanie licznikow polozenia

	void start_synchro(int drive_number);

	void finish_synchro(int drive_number);

	// oczekiwanie na przerwanie - tryb obslugi i delay(lag) po odebraniu przerwania
	int hi_int_wait(int inter_mode, int lag);

}; // koniec: class hardware_interface

#ifdef __cplusplus
extern "C"
{
#endif
    // pid_t far int_handler (void);  // Obsluga przerwania
    // by YOYEK & 7 - zastapic inna procedura obslugi prrzerwania

    const struct sigevent *
                int_handler (void *arg, int id); // by YOYEK & 7 - nowa forma z helpu

#ifdef __cplusplus
}
#endif


} // namespace irp6ot
} // namespace edp
} // namespace mrrocpp

#endif // __HI_RYDZ_H
