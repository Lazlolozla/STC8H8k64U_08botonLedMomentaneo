/* stc8h.h - STC8H8K64U SFR definitions (corregido) */
#ifndef STC8H_H
#define STC8H_H

#include <stdint.h>

/* ================================================================ */
/*                         PUERTOS                                  */
/* ================================================================ */

__sfr __at (0x80) P0;
__sfr __at (0x90) P1;
__sfr __at (0xA0) P2;
__sfr __at (0xB0) P3;
__sfr __at (0xC0) P4;

/* Bits de puertos direccionables */
__sbit __at (0xA0) P2_0;
__sbit __at (0xA1) P2_1;
__sbit __at (0xA2) P2_2;
__sbit __at (0xA3) P2_3;
__sbit __at (0xA4) P2_4;

__sbit __at (0xB0) P3_0;
__sbit __at (0xB1) P3_1;
__sbit __at (0xB2) P3_2;
__sbit __at (0xB3) P3_3;

/* ================================================================ */
/*               CONFIGURACION DE MODOS DE PUERTO                   */
/* ================================================================ */

/* Modo 00 = Cuasi-bidireccional (pull-up interno) */
/* Modo 01 = Push-pull          */
/* Modo 10 = Entrada solo       */
/* Modo 11 = Drenaje abierto    */

__sfr __at (0x93) P0M1;
__sfr __at (0x94) P0M0;
__sfr __at (0x91) P1M1;
__sfr __at (0x92) P1M0;
__sfr __at (0x95) P2M1;
__sfr __at (0x96) P2M0;
__sfr __at (0xB1) P3M1;
__sfr __at (0xB2) P3M0;
__sfr __at (0xB3) P4M1;   /* Nota: verificar en tu modelo especifico */
__sfr __at (0xB4) P4M0;

/* ================================================================ */
/*                         UART0                                    */
/* ================================================================ */

__sfr __at(0x98) SCON;
__sfr __at(0x99) SBUF;

/* ================================================================ */
/*                         TIMERS                                   */
/* ================================================================ */

__sfr __at(0x88) TCON;
__sfr __at(0x89) TMOD;

__sfr __at(0x8A) TL0;
__sfr __at(0x8C) TH0;
__sfr __at(0x8B) TL1;
__sfr __at(0x8D) TH1;

__sfr __at(0xD6) T2H;
__sfr __at(0xD7) T2L;
__sfr __at(0xD2) T2MOD;

/* ================================================================ */
/*                      REGISTROS AUXILIARES                        */
/* ================================================================ */

__sfr __at(0x8E) AUXR;

/* Bits de AUXR para Timer2 como baudrate UART1 */
/* Bit 0: S1ST2 = 1  -> UART1 usa Timer2      */
/* Bit 2: T2x12 = 1  -> Timer2 en modo 1T     */
/* Bit 4: T2R   = 1  -> Timer2 arranca        */

/* ================================================================ */
/*                     INTERRUPCIONES                               */
/* ================================================================ */

__sfr __at(0xA8) IE;

/* ================================================================ */
/*                       BITS DIRECCIONABLES                        */
/* ================================================================ */

/* TCON bits */
__sbit __at(0x8C) TR0;      /* TCON.4 */
__sbit __at(0x8D) TF0;      /* TCON.5 */
__sbit __at(0x8E) TR1;      /* TCON.6  <- CORREGIDO (antes 0x8F) */
__sbit __at(0x8F) TF1;      /* TCON.7  <- CORREGIDO (antes 0x8D) */

/* SCON bits */
__sbit __at(0x99) TI;       /* SCON.1 */
__sbit __at(0x98) RI;       /* SCON.0 */

/* IE bits */
__sbit __at(0xA9) ET0;      /* IE.1 */
__sbit __at(0xAF) EA;        /* IE.7 */

/* ================================================================ */
/*                       CONSTANTES DE TIMERS                       */
/* ================================================================ */

/* Timer2: 9600 baud @ 24 MHz, modo 1T (T2x12=1) */
/* Formula: 65536 - (FOSC / (BaudRate * 4))      */
/*          65536 - (24000000 / (9600 * 4)) = 65536 - 625 = 64911 = 0xFD8F */
#define TIMER2_T2H     0xFD
#define TIMER2_T2L     0x8F

#endif /* STC8H_H */
