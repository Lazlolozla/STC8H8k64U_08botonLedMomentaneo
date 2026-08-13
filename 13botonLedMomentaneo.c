/* 08botonLedMomentaneo.c - Control de LED con pulsador momentaneo */
/* STC8H8K64U @ 24MHz - Anti-rebote por conteo                     */
/* 
 * FUNCIONALIDAD:
 * - Al presionar P3.2: LED (P2.0) se ENCIENDE, UART reporta "PRESIONADO"
 * - Al soltar P3.2: LED se APAGA, UART reporta "LIBERADO"
 * - UART1 a 9600 baud via Timer2
 */

#include "stc8h.h"

/* ================================================================ */
/*                         DEFINICIONES                             */
/* ================================================================ */

#define BTN_PRESIONADO      0
#define BTN_LIBERADO        1

#define UMBRAL_ANTIRREBOTE  20     /* ~20ms a 24MHz */

/* LED con logica negativa (0 = encendido, 1 = apagado) */
#define LED_ENCENDER()      (P2_0 = 0)
#define LED_APAGAR()        (P2_0 = 1)

/* Boton con pull-up: 0 = presionado, 1 = liberado */
#define BTN_LEER()          (P3_2 == 0)

/* ================================================================ */
/*                       VARIABLES GLOBALES                         */
/* ================================================================ */

uint8_t  estado_led = 0;
uint16_t contador_estable = 0;
uint8_t  estado_boton_anterior = BTN_LIBERADO;

/* ================================================================ */
/*                     FUNCIONES UART                               */
/* ================================================================ */

void uart1_inicializar(void) {
    /* P3.0 (RXD) y P3.1 (TXD) en modo cuasi-bidireccional */
    /* La UART los controla automaticamente, no deben ser push-pull */
    P3M1 &= ~((1 << 0) | (1 << 1));
    P3M0 &= ~((1 << 0) | (1 << 1));
    
    /* Cargar valor de recarga para 9600 baud @ 24MHz, Timer2 1T */
    T2H = TIMER2_T2H;
    T2L = TIMER2_T2L;
    
    /* AUXR: S1ST2=1 (bit0), T2x12=1 (bit2), T2R=1 (bit4) */
    /* UART1 usa Timer2 como generador de baudrate en modo 1T */
    AUXR |= (1 << 0) | (1 << 4) | (1 << 2);
    
    /* UART1 Modo 1 (8N1), recepcion habilitada */
    SCON = 0x50;        /* SM0=0, SM1=1, REN=1 */
    TI = 0;
    RI = 0;
}

void uart1_enviar_byte(uint8_t dato) {
    SBUF = dato;
    while (TI == 0);    /* Esperar fin de transmision */
    TI = 0;             /* Limpiar flag */
}

void uart1_enviar_str(const uint8_t *str) {
    while (*str) {
        uart1_enviar_byte(*str);
        str++;
    }
}


void uart1_enviar_estado_boton(uint8_t estado) {
    if (estado == BTN_LIBERADO) {
        uart1_enviar_str("LIBERADO");
    } else {
        uart1_enviar_str("PRESIONADO");
    }
}

void uart1_enviar_estado_led(uint8_t estado) {
    if (estado == 0) {
        uart1_enviar_str("APAGADO");
    } else {
        uart1_enviar_str("ENCENDIDO");
    }
}

/* ================================================================ */
/*                     FUNCIONES DE RETARDO                         */
/* ================================================================ */

void delay_ms(uint16_t ms) {
    volatile uint16_t i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 2400; j++); 
    }
}

/* ================================================================ */
/*                     INICIALIZACION HARDWARE                      */
/* ================================================================ */

void inicializar_hardware(void) {
    /* LED en P2.0: modo cuasi-bidireccional, apagado (logica negativa) */
    P2M1 &= ~(1 << 0);
    P2M0 &= ~(1 << 0);
    LED_APAGAR();
    
    /* Boton en P3.2: modo cuasi-bidireccional con pull-up interno */
    P3M1 &= ~(1 << 2);
    P3M0 &= ~(1 << 2);
    P3_2 = 1;               /* Activar pull-up interno */
    
    /* Estado inicial */
    estado_led = 0;
    estado_boton_anterior = BTN_LIBERADO;
    contador_estable = 0;
}

/* ================================================================ */
/*                         FUNCION MAIN                             */
/* ================================================================ */

void main(void) {
    inicializar_hardware();
    uart1_inicializar();
    
    /* Mensaje de inicio */
    uart1_enviar_str("\r\n");
    uart1_enviar_str("=== STC8H PULSADOR MOMENTANEO ===\r\n");
    uart1_enviar_str("Esperando boton...\r\n");
    
    while (1) {
        uint8_t boton_presionado = BTN_LEER();
        
        if (boton_presionado) {
            /* Boton esta siendo presionado */
            if (contador_estable < UMBRAL_ANTIRREBOTE) {
                contador_estable++;
            } else {
                /* Flanco de activacion confirmado (anti-rebote superado) */
                if (estado_boton_anterior != BTN_PRESIONADO) {
                    estado_boton_anterior = BTN_PRESIONADO;

                    LED_ENCENDER();
                    estado_led = 1;
                    
                    /* Enviar reporte UART (SOLO UNA VEZ) */
                    uart1_enviar_estado_boton(BTN_PRESIONADO);
                    uart1_enviar_str(" - LED ");
                    uart1_enviar_estado_led(estado_led);
                    uart1_enviar_str("\r\n");
                }
                /* Si ya estaba presionado, no hacer nada (evita spam) */
            }
        } else {
            /* Boton liberado */
            contador_estable = 0;
            
            /* Flanco de liberacion confirmado */
            if (estado_boton_anterior != BTN_LIBERADO) {
                estado_boton_anterior = BTN_LIBERADO;
                
                LED_APAGAR();
                estado_led = 0;
                
                /* Enviar reporte UART (SOLO UNA VEZ) */
                uart1_enviar_estado_boton(BTN_LIBERADO);
                uart1_enviar_str(" - LED ");
                uart1_enviar_estado_led(estado_led);
                uart1_enviar_str("\r\n");
            }
            /* Si ya estaba liberado, no hacer nada */
        }
    }
}
