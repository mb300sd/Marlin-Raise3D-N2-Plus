/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"

#if NUM_SERIAL > 1
  #include "../../gcode/queue.h"
#endif

/**
 * M105: Read hot end and bed temperature
 */
void GcodeSuite::M105() {

  const int8_t target_extruder = get_target_extruder_from_command();
  if (target_extruder < 0) return;

  #if NUM_SERIAL > 1
    const int16_t port = command_queue_port[cmd_queue_index_r];
  #endif

  #if HAS_TEMP_SENSOR
    SERIAL_ECHOPGM_P(port, MSG_OK);
      #ifdef N_SERIES_PROTOCOL
        SERIAL_ECHOPGM_P(port, " T:");
        SERIAL_ECHO_F_P(port, thermalManager.degHotend(target_extruder), 1);
        SERIAL_ECHOPGM_P(port, " /");
        SERIAL_ECHO_F_P(port, (double)thermalManager.degTargetHotend(target_extruder), 1);
        SERIAL_ECHOPGM_P(port, " B:");
        SERIAL_ECHO_F_P(port, thermalManager.degBed(), 1);
        SERIAL_ECHOPGM_P(port, " /");
        SERIAL_ECHO_F_P(port, (double)thermalManager.degTargetBed(), 1);
        HOTEND_LOOP() {
        //for (int8_t e = 0; e < 2; e++) { /* always print for 2 even if not DUAL */
        SERIAL_ECHOPGM_P(port, " T");
        SERIAL_ECHO_P(port, e);
        SERIAL_ECHOPGM_P(port, ":");
        SERIAL_ECHO_F_P(port, thermalManager.degHotend(e), 1);
        SERIAL_ECHOPGM_P(port, " /");
        SERIAL_ECHO_F_P(port, (double)thermalManager.degTargetHotend(e), 1);
        SERIAL_ECHOPGM_P(port, " F");
        SERIAL_CHAR_P(port, '0' + e);
        SERIAL_CHAR_P(port, ':');
        SERIAL_ECHO_P(port, planner.flow_percentage[e]);
      }
      SERIAL_ECHOPGM_P(port, " S:");
      SERIAL_ECHO_P(port, fan_speed[0]);
      SERIAL_ECHOPGM_P(port, " P:");
      SERIAL_ECHO_P(port, feedrate_percentage);
    #else
      thermalManager.print_heater_states(target_extruder
        #if NUM_SERIAL > 1
          , port
        #endif
      );
    #endif
  #else // !HAS_TEMP_SENSOR
    SERIAL_ERROR_MSG_P(port, MSG_ERR_NO_THERMISTORS);
  #endif

  SERIAL_EOL_P(port);
}
