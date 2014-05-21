/*
 * Copyright (C) 2014 FU Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @defgroup    board_agilefox agilefox
 * @ingroup     boards
 * @brief       Board specific files for the agilefox board.
 * @{
 *
 * @file        board.h
 * @brief       Board specific definitions for the agilefox board.
 *
 * @author      Alaeddine Weslati <alaeddine.weslati@inria.fr>
 * @author      Thomas Eichinger <thomas.eichinger@fu-berlin.de>
 * @author      Oliver Hahm <oliver.hahm@inria.fr>
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <leds.h>

/**
 * Assign the UART baudrate
 */
#define UART_BAUDRATE   (115200)

/**
 * Define the type for the radio packet length for the transceiver
 */
typedef uint8_t radio_packet_length_t;

/**
 * Assign the hardware timer
 */
#define HW_TIMER            TIMER_0

#endif /* BOARD_H_ */
/** @} */
