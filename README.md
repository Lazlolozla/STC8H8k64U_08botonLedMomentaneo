STC8H Pulsador Momentáneo con LED y UART
Control de LED mediante pulsador momentáneo con reporte de estado por UART para microcontroladores STC8H8K64U (y familia compatible).
Descripción
Este proyecto implementa un sistema de control de LED con pulsador momentáneo que incluye:

    Entrada: Pulsador en P3.2 con pull-up interno y anti-rebote por software
    Salida: LED en P2.0 con lógica negativa (0 = encendido, 1 = apagado)
    Comunicación: UART1 a 9600 baud vía Timer2 para reporte de eventos
    Anti-rebote: Algoritmo de conteo estable con umbral configurable

Comportamiento
Estado del botón	LED	Mensaje UART
Presionado (P3.2 = 0)	Encendido	PRESIONADO - LED ENCENDIDO
Liberado (P3.2 = 1)	Apagado	LIBERADO - LED APAGADO
Los mensajes se envían una sola vez por cambio de estado (detección de flanco), evitando spam en la consola serial.
Hardware
Requisitos

    Microcontrolador STC8H8K64U (o compatible de la familia STC8H)
    Oscilador interno o externo a 24 MHz
    Adaptador USB-TTL  Pl2303 para comunicación serial
    LED con resistencia limitadora de corriente
    Pulsador momentáneo

Conexiones
Table
Pin STC8H	Función	Conexión
P3.1 (TXD)	Transmisión UART	RXD del adaptador USB-TTL
P3.0 (RXD)	Recepción UART	TXD del adaptador USB-TTL
GND	Tierra común	GND del adaptador USB-TTL
P3.2	Entrada del pulsador	Pulsador a GND (pull-up interno)
P2.0	Salida del LED	Cátodo del LED (ánodo a VCC)

    Nota: El LED utiliza lógica negativa. Conectar el ánodo del LED a VCC y el cátodo a P2.0 a través de una resistencia de 220Ω–1kΩ.

Software
Estructura del proyecto
plain

.
├── stc8h.h                 # Definiciones de registros SFR
├── 08botonLedMomentaneo.c  # Código fuente principal
├── README.md               # Este archivo
├── LICENSE                 # Licencia Unlicense
└── .gitignore              # Archivos ignorados por Git

Compilación
Este proyecto utiliza el compilador SDCC (Small Device C Compiler).
bash

# Compilar
sdcc 08botonLedMomentaneo.c

# El resultado genera archivos .ihx, .rel, .lst, etc.
# El archivo para programar es: 08botonLedMomentaneo.ihx

Programación
Utilizar stcgal o la herramienta oficial de STC para cargar el firmware:
bash

# Con stcgal (recomendado en Linux)
stcgal -P stc8 -p /dev/ttyUSB0 -b 9600 08botonLedMomentaneo.ihx

# En Windows (con STC-ISP)
# Usar la herramienta gráfica oficial de STC Micro

Monitor serial
bash

# Linux/macOS
picocom -b 9600 /dev/ttyUSB0

# Windows (PuTTY)
# Configurar: COMx, 9600 baud, 8 data bits, 1 stop bit, no parity, no flow control

Configuración del UART

Parámetro	Valor
Baudrate	9600
Modo	8N1 (8 data bits, no parity, 1 stop bit)
Generador	Timer2 en modo 1T
Fórmula	65536 - (FOSC / (BaudRate × 4))
Valor de recarga	0xFD8F (TH2=0xFD, TL2=0x8F)
Registros SFR utilizados
Registro	Dirección	Función
P2	0xA0	Puerto 2 (LED)
P3	0xB0	Puerto 3 (botón y UART)
P3M1	0xB1	Modo alto puerto 3
P3M0	0xB2	Modo bajo puerto 3
P2M1	0x95	Modo alto puerto 2
P2M0	0x96	Modo bajo puerto 2
SCON	0x98	Control UART
SBUF	0x99	Buffer de datos UART
T2H	0xD6	Timer2 high byte
T2L	0xD7	Timer2 low byte
AUXR	0x8E	Registro auxiliar
Bits de AUXR
Bit	Nombre	Descripción
0	S1ST2	UART1 usa Timer2 como generador de baudrate
2	T2x12	Timer2 en modo 1T (1) o 12T (0)
4	T2R	Timer2 run (arrancar/detener)
Anti-rebote
El algoritmo implementa un contador estable que debe alcanzar un umbral antes de confirmar un cambio de estado:

    Umbral: 20 ciclos de muestreo
    Frecuencia de muestreo: 1 kHz (delay de 1 ms)
    Tiempo de confirmación: ~20 ms
    Histeresis: El contador decrementa al liberar, evitando falsos positivos

Licencia
Este proyecto está liberado al dominio público bajo la licencia Unlicense.
Consulta el archivo LICENSE para más detalles.
Desarrollado para microcontroladores STC8H de STC Microelectronics.
Recursos útiles

    STC Microelectronics - Sitio oficial
    SDCC - Small Device C Compiler
    stcgal - Herramienta de programación STC
    Datasheet STC8H (2022/3/9)
