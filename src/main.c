#include "derivative.h" /* include peripheral declarations */
#include <stdio.h>
#include <stdlib.h>

const int NDIGITS = 4;

const int pin_led_red = 0x20000;	//C17
const int pin_led_green = 0x10000;  //C16
const int pin_led_yellow = 0x2000;	//C13
const int pin_led_blue = 0x1000;	//C12

const int pin_button_red = 0x800;		//C11
const int pin_button_green = 0x400;  	//C10
const int pin_button_yellow = 0x40;		//C6
const int pin_button_blue = 0x20;		//C5

const int pin_1_display = 0x20;			//E5
const int pin_2_display = 0x10;			//E4
const int pin_3_display = 0x8;			//E3
const int pin_4_display = 0x4;			//E2
const int pin_5_display = 0x800;		//B11
const int pin_6_display = 0x400;		//B10
const int pin_7_display = 0x200;		//B9
const int pin_8_display = 0x100;		//B8
const int pin_9_display	= 0x1;			//B0
const int pin_10_display = 0x2;			//B1
const int pin_11_display = 0x800000;	//E23
const int pin_12_display = 0x8;			//B3
const int pin_13_display = 0x4;			//B2
const int pin_14_display = 0x400000;	//E22
const int pin_15_display = 0x200000;	//E21
const int pin_16_display = 0x100000;	//E20

void retard(int n);
void GeneraSequencia(int *sequencia,int tamany_sequencia);
int  SequenciaUsuari(int *sequencia_user, int tamany_sequencia);
int ComprobaSequencia(int *sequencia,int *SequenciaJugador, int tamany_sequencia);
void PulsaJugador();
void ContadorPunts();
void EnvioI2C();
void ResultatIncorrecte();

//Inicializa todos los periféricos
void init();

//Actualitza la pantalla con los datos de la puntuación del jugador
void updateScore(int score, int display[4][2], int nRows, int nCols);



int main (void) 
{
	int tamany_sequencia = 5;
	int punts;
	int sequencia[tamany_sequencia];
	int sequencia_user[tamany_sequencia];
	int error = 1;
	
	init();
	
	
	while (error == 1)
	{
		GeneraSequencia(sequencia,tamany_sequencia);
		SequenciaUsuari(sequencia_user, tamany_sequencia);
		error = ComprobaSequencia(sequencia, sequencia_user, tamany_sequencia);
	}
	
	led_on(pin_led_red);
	
	return 0;
}

void init()
{
	// Configuración del reloj del sistema
		SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;		// Habilitamos el clock en el puerto B
		SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;		// Habilitamos el clock en el puerto C
		SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;		// Habilitamos el clock en el puerto E
		
		// Configuramos el pin como  GPIO
		PORTC_PCR12 = 0x100;
		PORTC_PCR13 = 0x100;
		PORTC_PCR16 = 0x100;
		PORTC_PCR17 = 0x100;
		
		PORTC_PCR11 = 0x100;
		PORTC_PCR10 = 0x100;
		PORTC_PCR6 = 0x100;
		PORTC_PCR5 = 0x100;
		
		PORTE_PCR2 = 0x100;
		PORTE_PCR3 = 0x100;
		PORTE_PCR4 = 0x100;
		PORTE_PCR5 = 0x100;
		PORTE_PCR20 = 0x100;
		PORTE_PCR21 = 0x100;
		PORTE_PCR22 = 0x100;
		PORTE_PCR23 = 0x100;
		PORTB_PCR0 = 0x100;
		PORTB_PCR1 = 0x100;
		PORTB_PCR2 = 0x100;
		PORTB_PCR3 = 0x100;
		PORTB_PCR8 = 0x100;
		PORTB_PCR9 = 0x100;
		PORTB_PCR10 = 0x100;
		PORTB_PCR11 = 0x100;
		
		// Configuramos el pin como salida
		GPIOC_PDDR |= (pin_led_red | pin_led_green | pin_led_yellow | pin_led_blue);
		GPIOE_PDDR |= (pin_1_display | pin_2_display | pin_3_display | pin_4_display
						| pin_11_display | pin_14_display | pin_15_display | pin_16_display);
		GPIOB_PDDR |= (pin_5_display | pin_6_display | pin_7_display | pin_8_display
						| pin_9_display | pin_10_display | pin_12_display);
		
		//Configuramos los pines como entrada
		GPIOC_PDDR &= ~pin_button_red;
		GPIOC_PDDR &= ~pin_button_green;
		GPIOC_PDDR &= ~pin_button_yellow;
		GPIOC_PDDR &= ~pin_button_blue;
}

void updateScore(int score, int display[4][2], int nRows, int nCols) {
  int score_array[nRows];

  if (score > 999)
    score_array[0] = score / 1000; // millares
  else
    score_array[0] = 0;
  if (score > 99)
    score_array[1] = (score % 1000) / 100; // centenas
  else
    score_array[1] = 0;
  if (score > 9) {
    score_array[2] = (score % 100) / 10; // decenas
    score_array[3] = score % 10;         // unidades
  } else {
    score_array[2] = 0;     // decenas
    score_array[3] = score; // unidades
  }

  for (int i = 0; i < nRows; i++) {
    // Para los dígitos
    switch (i) {
    case 0:
      display[i][0] = (pin_8_display | pin_6_display);
      display[i][1] = pin_2_display;
      break;
    case 1:
      display[i][0] = (pin_8_display | pin_6_display);
      display[i][1] = pin_1_display;
      break;
    case 2:
      display[i][0] = pin_8_display;
      display[i][1] = (pin_2_display | pin_1_display);
      break;
    case 3:
      display[i][0] = pin_6_display;
      display[i][1] = (pin_2_display | pin_1_display);
      break;
    }
    // Para los números
    switch (score_array[i]) {
    case 0:
      display[i][0] |= (pin_13_display | pin_5_display);
      display[i][1] |= (pin_16_display | pin_14_display | pin_11_display | pin_3_display);
      break;
    case 1:
      display[i][0] |= pin_13_display;
      display[i][1] |= pin_16_display;
      break;
    case 2:
      display[i][0] |= pin_5_display;
      display[i][1] |= (pin_16_display | pin_15_display | pin_14_display | pin_3_display);
      break;
    case 3:
      display[i][0] |= pin_13_display;
      display[i][1] |= (pin_16_display | pin_15_display | pin_14_display | pin_3_display);
      break;
    case 4:
      display[i][0] |= pin_13_display;
      display[i][1] |= (pin_16_display | pin_15_display | pin_11_display);
      break;
    case 5:
      display[i][0] |= pin_13_display;
      display[i][1] |= (pin_15_display | pin_14_display | pin_11_display | pin_3_display);
      break;
    case 6:
      display[i][0] |= (pin_13_display | pin_5_display);
      display[i][1] |= (pin_14_display | pin_11_display | pin_3_display);
      break;
    case 7:
      display[i][0] |= pin_13_display;
      display[i][1] |= (pin_16_display | pin_14_display);
      break;
    case 8:
      display[i][0] |= (pin_13_display | pin_5_display);
      display[i][1] |= (pin_16_display | pin_15_display | pin_14_display |
                        pin_11_display | pin_3_display);
      break;
    case 9:
      display[i][0] |= pin_13_display;
      display[i][1] |= (pin_16_display | pin_15_display | pin_14_display | pin_11_display);
      break;
    }
  }
}

void led_on(int pin)
{
	GPIOC_PSOR = pin;
}

void led_off(int pin)
{
	GPIOC_PCOR = pin;
}

// funcio retard
void retard(int n) {
int i;
int j;
	for(i = 0 ; i < n; i++)
		for (j = 0; j < 7000; j++) {}
}

void GeneraSequencia(int *sequencia,int tamany_sequencia)
{
	int random_num;
	int i;
	
	for(i = 0; i < tamany_sequencia; i++)
	{
		random_num = rand() % 4;

		switch (random_num)
		{
			case 0: led_on(pin_led_red);
					retard(250);
					led_off(pin_led_red);
					retard(250);
			break;
			case 1: led_on(pin_led_green);
					retard(250);
					led_off(pin_led_green);
					retard(250);
			break;
			case 2: led_on(pin_led_yellow);
					retard(250);
					led_off(pin_led_yellow);
					retard(250);
			break;
			case 3: led_on(pin_led_blue);
					retard(250);
					led_off(pin_led_blue);
					retard(250);
			break;
		}
		sequencia[i] = random_num;
	}
}
int  SequenciaUsuari(int *sequencia_jugador, int tamany_sequencia)
{
	int i = 0;
	while( i < tamany_sequencia)
	{
		if (GPIOC_PDIR & pin_button_red  )
			GPIOC_PCOR = pin_led_red;
		else
		{
			led_on(pin_led_red);
			sequencia_jugador[i] = 0;
			i++;
			retard(250);
			led_off(pin_led_red);
			retard(250);
			continue;
		}
		
		if (GPIOC_PDIR & pin_button_green  )
		{
			GPIOC_PCOR = pin_led_green;
		}
		else
		{
			led_on(pin_led_green);
			sequencia_jugador[i] = 1;
			i++;
			retard(250);
			led_off(pin_led_green);
			retard(250);
			continue;
		}
		
		if (GPIOC_PDIR & pin_button_yellow  )
		{
			GPIOC_PCOR = pin_led_yellow;
		}
		else
		{
			led_on(pin_led_yellow);
			sequencia_jugador[i] = 2;
			i++;
			retard(250);
			led_off(pin_led_yellow);
			retard(250);
			continue;
		}
		
		if (GPIOC_PDIR & pin_button_blue  )
		{
			GPIOC_PCOR = pin_led_blue;
		}
		else
		{
			led_on(pin_led_blue);		
			sequencia_jugador[i] = 3;
			i++;
			retard(250);
			led_off(pin_led_blue);
			retard(250);
			continue;			
		}

	}
}


int ComprobaSequencia(int *sequencia,int *SequenciaJugador, int tamany_sequencia)
{
	int i;
	for(i = 0; i < tamany_sequencia; i++)
	{
		if (sequencia[i] != SequenciaJugador[i])
			return 0;
	}
	
	retard(250);
	
	led_on(pin_led_red);
	led_on(pin_led_green);
	led_on(pin_led_yellow);
	led_on(pin_led_blue);
	
	retard(250);
	
	led_off(pin_led_red);
	led_off(pin_led_green);
	led_off(pin_led_yellow);
	led_off(pin_led_blue);
	
	return 1;
}
void PulsaJugador()
{
	
}


void ContadorPunts()
{
	
}

void EnvioI2C()
{
		
}

void ResultatIncorrecte()
{
	
}
