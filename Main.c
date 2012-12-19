#include <p30F6014a.h>
#include <libpic30.h>
#include <adc12.h>
#include "A34760.h"
#include "A34760_PINS.h"
#include "IOPorts.h"
#include "LTC2656.h"
#include <i2c.h>
#include "MCP23017.h"
#include "ETMSPI.h"
#include <spi.h>
#include "ETMdsp.h"
#include "config.h"



_prog_addressT EE_address_ps_magnet_config_in_EEPROM;
unsigned int _EEDATA(32) ps_magnet_config_in_EEPROM[] = PS_MAGNET_DEFAULT_CONFIG;  // Create 16 word structure in EEPROM and load default configuration values
unsigned int ps_magnet_config_ram_copy[16];

_prog_addressT EE_address_ps_filament_config_in_EEPROM;
unsigned int _EEDATA(32) ps_filament_config_in_EEPROM[] = PS_FILAMENT_DEFAULT_CONFIG;  // Create 16 word structure in EEPROM and load default configuration values
unsigned int ps_filament_config_ram_copy[16];

_prog_addressT EE_address_ps_thyr_cathode_htr_config_in_EEPROM;
unsigned int _EEDATA(32) ps_thyr_cathode_htr_config_in_EEPROM[] = PS_THYR_CATH_HTR_DEFAULT_CONFIG;  // Create 16 word structure in EEPROM and load default configuration values
unsigned int ps_thyr_cathode_htr_config_ram_copy[16];

_prog_addressT EE_address_ps_thyr_reservoir_htr_config_in_EEPROM;
unsigned int _EEDATA(32) ps_thyr_reservoir_htr_config_in_EEPROM[] = PS_THYR_RESER_HTR_DEFAULT_CONFIG;  // Create 16 word structure in EEPROM and load default configuration values
unsigned int ps_thyr_reservoir_htr_config_ram_copy[16];


_prog_addressT EE_address_ps_hv_lambda_mode_A_config_in_EEPROM;
unsigned int _EEDATA(32) ps_hv_lambda_mode_A_config_in_EEPROM[] = PS_HV_LAMBDA_MODE_A_DEFAULT_CONFIG;  // Create 16 word structure in EEPROM and load default configuration values
unsigned int ps_hv_lambda_mode_A_config_ram_copy[16];

_prog_addressT EE_address_ps_hv_lambda_mode_B_config_in_EEPROM;
unsigned int _EEDATA(32) ps_hv_lambda_mode_B_config_in_EEPROM[] = PS_HV_LAMBDA_MODE_B_DEFAULT_CONFIG;  // Create 16 word structure in EEPROM and load default configuration values
unsigned int ps_hv_lambda_mode_B_config_ram_copy[16];


_prog_addressT EE_address_ps_magnetron_mode_A_config_in_EEPROM;
unsigned int _EEDATA(32) ps_magnetron_mode_A_config_in_EEPROM[] = PS_MAGNETRON_MODE_A_DEFAULT_CONFIG;  // Create 16 word structure in EEPROM and load default configuration values
unsigned int ps_magnetron_mode_A_config_ram_copy[16];

_prog_addressT EE_address_ps_magnetron_mode_B_config_in_EEPROM;
unsigned int _EEDATA(32) ps_magnetron_mode_B_config_in_EEPROM[] = PS_MAGNETRON_MODE_B_DEFAULT_CONFIG;  // Create 16 word structure in EEPROM and load default configuration values
unsigned int ps_magnetron_mode_B_config_ram_copy[16];

_prog_addressT EE_address_pulse_counter_repository_in_EEPROM;
unsigned int _EEDATA(32) pulse_counter_repository_in_EEPROM[] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};  // Create 16 word structure in EEPROM and load zeros
unsigned int pulse_counter_repository_ram_copy[16];


_prog_addressT EE_address_control_loop_cal_data_in_EEPROM;
unsigned int _EEDATA(32) control_loop_cal_data_in_EEPROM[] = CONTROL_LOOP_CAL_DATA_DEFAULT_CONFIG; // Create 16 word structure in EEPROM and load default configuration values
unsigned int control_loop_cal_data_ram_copy[16];


/*
  
  
 */



/*
  Pulse Counter Structure
  pulse_counter_repository_ram_copy[0] = pulse_counter MSB
  pulse_counter_repository_ram_copy[1] = pulse_counter 
  pulse_counter_repository_ram_copy[2] = pulse_counter 
  pulse_counter_repository_ram_copy[3] = pulse_counter LSB

  pulse_counter_repository_ram_copy[4] = arc_counter MSB
  pulse_counter_repository_ram_copy[5] = arc_counter LSB


*/



/* Configuration Bit Settings */
//_FOSC(LPRC & CSW_FSCM_OFF);
_FOSC(ECIO & CSW_FSCM_OFF); // Primary Oscillator with PLL and Startup with User Selected Oscillator Source
// DPARKER this is for use with 40MHz ext clock

//_FOSC(FRC_PLL16 & CSW_FSCM_OFF); // Primary Oscillator with PLL and Startup with User Selected Oscillator Source
                                 // External Oscillator is 7.3738MHz with 16x PLL and 4x Clocks per Instruction cycle
                                 // This yields an Instruction Cycle Frequency of 29.4952 MHz
                                 // This yields an Instruction Cycle Time of 33.904nS


// Watchdog Timeout is 2 Millisconds with no pre scallers
_FWDT(WDT_ON & WDTPSA_1 & WDTPSB_16);  // Watchdog Timer is enabled, 32ms TIMEOUT
//_FWDT(WDT_OFF & WDTPSA_1 & WDTPSB_16);  // Watchdog Timer is disnabled, 32ms TIMEOUT


//_FBORPOR(PWRT_64 & BORV_27 & PBOR_ON & MCLR_EN); // Brown out and Power on Timer settings
_FBORPOR(PWRT_64 & BORV_45 & PBOR_OFF & MCLR_EN); // Brown out and Power on Timer settings

// _FBS()  // No Boot Segment

_FBS(WR_PROTECT_BOOT_OFF & NO_BOOT_CODE & NO_BOOT_EEPROM & NO_BOOT_RAM);

_FSS(WR_PROT_SEC_OFF & NO_SEC_CODE & NO_SEC_EEPROM & NO_SEC_RAM);

_FGS(CODE_PROT_OFF);

_FICD(PGD);

int main(void) {

  control_state = STATE_START_UP;

  _init_prog_address(EE_address_ps_magnet_config_in_EEPROM, ps_magnet_config_in_EEPROM);
  _memcpy_p2d16(ps_magnet_config_ram_copy, EE_address_ps_magnet_config_in_EEPROM, _EE_ROW);

  _init_prog_address(EE_address_ps_filament_config_in_EEPROM, ps_filament_config_in_EEPROM);
  _memcpy_p2d16(ps_filament_config_ram_copy, EE_address_ps_filament_config_in_EEPROM, _EE_ROW);

  _init_prog_address(EE_address_ps_thyr_cathode_htr_config_in_EEPROM, ps_thyr_cathode_htr_config_in_EEPROM);
  _memcpy_p2d16(ps_thyr_cathode_htr_config_ram_copy, EE_address_ps_thyr_cathode_htr_config_in_EEPROM, _EE_ROW);

  _init_prog_address(EE_address_ps_thyr_reservoir_htr_config_in_EEPROM, ps_thyr_reservoir_htr_config_in_EEPROM);
  _memcpy_p2d16(ps_thyr_reservoir_htr_config_ram_copy, EE_address_ps_thyr_reservoir_htr_config_in_EEPROM, _EE_ROW);

  _init_prog_address(EE_address_ps_hv_lambda_mode_A_config_in_EEPROM, ps_hv_lambda_mode_A_config_in_EEPROM);
  _memcpy_p2d16(ps_hv_lambda_mode_A_config_ram_copy, EE_address_ps_hv_lambda_mode_A_config_in_EEPROM, _EE_ROW);

  _init_prog_address(EE_address_ps_hv_lambda_mode_B_config_in_EEPROM, ps_hv_lambda_mode_B_config_in_EEPROM);
  _memcpy_p2d16(ps_hv_lambda_mode_B_config_ram_copy, EE_address_ps_hv_lambda_mode_B_config_in_EEPROM, _EE_ROW);

  _init_prog_address(EE_address_ps_magnetron_mode_A_config_in_EEPROM, ps_magnetron_mode_A_config_in_EEPROM);
  _memcpy_p2d16(ps_magnetron_mode_A_config_ram_copy, EE_address_ps_magnetron_mode_A_config_in_EEPROM, _EE_ROW);

  _init_prog_address(EE_address_ps_magnetron_mode_B_config_in_EEPROM, ps_magnetron_mode_B_config_in_EEPROM);
  _memcpy_p2d16(ps_magnetron_mode_B_config_ram_copy, EE_address_ps_magnetron_mode_B_config_in_EEPROM, _EE_ROW);

  _init_prog_address(EE_address_pulse_counter_repository_in_EEPROM, pulse_counter_repository_in_EEPROM);
  _memcpy_p2d16(pulse_counter_repository_ram_copy, EE_address_pulse_counter_repository_in_EEPROM, _EE_ROW);

  _init_prog_address(EE_address_control_loop_cal_data_in_EEPROM, control_loop_cal_data_in_EEPROM);
  _memcpy_p2d16(control_loop_cal_data_ram_copy, EE_address_control_loop_cal_data_in_EEPROM, _EE_ROW);

  ram_config_set_magnetron_magnet_current_from_GUI = 0;

  while (1) {
    DoStateMachine();
  }
}
