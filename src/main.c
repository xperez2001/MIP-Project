#include "derivative.h" /* include peripheral declarations */
#include <stdio.h>
#include <stdlib.h>

const int pin_led_red = 0x20000;	//C17
const int pin_led_green = 0x10000;  //C16
const int pin_led_yellow = 0x2000;	//C13
const int pin_led_blue = 0x1000;	//C12

const int pin_button_red = 0x800;		//C11
const int pin_button_green = 0x400;  	//C10
const int pin_button_yellow = 0x40;		//C6
const int pin_button_blue = 0x20;		//C5

void retard(int n);
void GeneraSequencia(int *sequencia,int tamany_sequencia);
int  SequenciaUsuari(int *sequencia_user, int tamany_sequencia);
int ComprobaSequencia(int *sequencia,int *SequenciaJugador, int tamany_sequencia);
void PulsaJugador();
void ContadorPunts();
void EnvioI2C();
void ResultatIncorrecte();
void init();


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
		SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;		// Habilitamos el clock en el puerto C
		
		// Configuramos el pin como  GPIO
		PORTC_PCR12 = 0x100;
		PORTC_PCR13 = 0x100;
		PORTC_PCR16 = 0x100;
		PORTC_PCR17 = 0x100;
		
		PORTC_PCR11 = 0x100;
		PORTC_PCR10 = 0x100;
		PORTC_PCR6 = 0x100;
		PORTC_PCR5 = 0x100;
		
		// Configuramos el pin como salida
		GPIOC_PDDR |= pin_led_red;
		GPIOC_PDDR |= pin_led_green;
		GPIOC_PDDR |= pin_led_yellow;
		GPIOC_PDDR |= pin_led_blue;
		
		GPIOC_PDDR &= ~pin_button_red;
		GPIOC_PDDR &= ~pin_button_green;
		GPIOC_PDDR &= ~pin_button_yellow;
		GPIOC_PDDR &= ~pin_button_blue;
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
