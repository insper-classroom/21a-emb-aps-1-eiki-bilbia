/************************************************************************/
/* APS1 - Buzzer Musical                                                */
/* Beatriz Muniz de Castro e Silva                                      */
/*                                                                      */
/* 5 semestre - Eng. da Computa��o - Insper                             */
/* Rafael Corsi - rafael.corsi@insper.edu.br                            */
/*                                                                      */
/* M�sicas adquiridas no reposit�rio:                                   */
/* https://github.com/robsoncouto/arduino-songs                         */
/*																		*/
/************************************************************************/


/************************************************************************/
/* includes                                                             */
/************************************************************************/
#include "asf.h"
#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"
#include "notas_freq.h"
#include "string.h"


/************************************************************************/
/* defines                                                              */
/************************************************************************/


// As frequ�ncias das notas foram guardadas no arquivo notas_freq.h

// LED verde da placa
#define LED_PIO           PIOC
#define LED_PIO_ID        ID_PIOC
#define LED_PIO_IDX       8
#define LED_PIO_IDX_MASK  (1 << LED_PIO_IDX)

// Bot�o da placa SW300
#define BUT_PIO			  PIOA
#define BUT_PIO_ID		  ID_PIOA
#define BUT_PIO_IDX		  11
#define BUT_PIO_IDX_MASK  (1u << BUT_PIO_IDX)

// Perif�ricos do OLED1
// LED1
#define LED1_PIO           PIOA
#define LED1_PIO_ID        ID_PIOA
#define LED1_PIO_IDX       0
#define LED1_PIO_IDX_MASK  (1 << LED1_PIO_IDX)

// LED2
#define LED2_PIO           PIOC
#define LED2_PIO_ID        ID_PIOC
#define LED2_PIO_IDX       30
#define LED2_PIO_IDX_MASK  (1 << LED2_PIO_IDX)

// LED3
#define LED3_PIO           PIOB
#define LED3_PIO_ID        ID_PIOB
#define LED3_PIO_IDX       2
#define LED3_PIO_IDX_MASK  (1 << LED3_PIO_IDX)

// BT1
#define BUT1_PIO			PIOD
#define BUT1_PIO_ID		    ID_PIOD
#define BUT1_PIO_IDX		28
#define BUT1_PIO_IDX_MASK   (1u << BUT1_PIO_IDX)

// BT2
#define BUT2_PIO			PIOC
#define BUT2_PIO_ID		    ID_PIOC
#define BUT2_PIO_IDX		31
#define BUT2_PIO_IDX_MASK   (1u << BUT2_PIO_IDX)

// BT3
#define BUT3_PIO			PIOA
#define BUT3_PIO_ID		    ID_PIOA
#define BUT3_PIO_IDX		19
#define BUT3_PIO_IDX_MASK   (1u << BUT3_PIO_IDX)

// Buzzer conectado direto � placa
#define BUZZ_PIO			PIOD
#define BUZZ_PIO_ID		ID_PIOD
#define BUZZ_PIO_IDX		30
#define BUZZ_PIO_IDX_MASK (1 << BUZZ_PIO_IDX)

#define CONT_MAX	3
/************************************************************************/
/* constants                                                            */
/************************************************************************/

int contador = 1;

/************************************************************************/
/* structs                                                              */
/************************************************************************/


//fazer o struct aqui
typedef struct{
	int *melody;
	int tempo;
	char title[13];
	int size;
} song;

typedef struct{
	song songs[4];
	song curr_song;
} disc;


/************************************************************************/
/* m�sicas                                                              */
/************************************************************************/


#include "GoT.h"
#include "Mii.h"
#include "Mario.h"
#include "StarWars.h"

/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/

volatile int pause_flag = 1;

/************************************************************************/
/* prototypes                                                           */
/************************************************************************/


void init(void);
void tone(int freq, int time);
void back_song(disc* disc1);
void next_song(disc* disc1);
void play(song curr_song);




/************************************************************************/
/* interrupcoes                                                         */
/************************************************************************/

void but_callback(void){
 	pause_flag = !pause_flag;
}

/************************************************************************/
/* funcoes                                                              */
/************************************************************************/

void init(void)
{
	sysclk_init();
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	
	// abrindo todos os PIOs
	pmc_enable_periph_clk(ID_PIOA);
	pmc_enable_periph_clk(ID_PIOB);
	pmc_enable_periph_clk(ID_PIOC);
	pmc_enable_periph_clk(ID_PIOD);
	
	// configurando but e led da placa
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 0, 0, 0);
	

	// configurando leds da OLED1
	pio_set_output(LED1_PIO, LED1_PIO_IDX_MASK, 0, 0, 0);
	pio_set_output(LED2_PIO, LED2_PIO_IDX_MASK, 0, 0, 0);
	pio_set_output(LED3_PIO, LED3_PIO_IDX_MASK, 0, 0, 0);
	
	// configurando buts da OLED1
	pio_set_input(BUT1_PIO, BUT1_PIO_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT1_PIO, BUT1_PIO_IDX_MASK, 1);
	pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT2_PIO, BUT2_PIO_IDX_MASK, 1);
	pio_set_input(BUT3_PIO, BUT3_PIO_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT3_PIO, BUT3_PIO_IDX_MASK, 1);

	// configurando buzzer
	pio_set_output(BUZZ_PIO, BUZZ_PIO_IDX_MASK, 0, 0, 0);
	
	pio_configure(BUT_PIO, PIO_INPUT, BUT_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	// interrupt pra pausar
	pio_handler_set(BUT_PIO,
	BUT_PIO_ID,
	BUT_PIO_IDX_MASK,
	PIO_IT_FALL_EDGE,
	but_callback);
	
	
	
	
	NVIC_SetPriority(BUT_PIO_ID, 4); // Prioridade 4
	NVIC_EnableIRQ(BUT_PIO_ID);
	
	pio_enable_interrupt(BUT_PIO, BUT_PIO_IDX_MASK);
}


void tone(int freq, int time) {
	while(pause_flag==1){
		pio_clear(PIOA, LED1_PIO_IDX_MASK);
		pio_clear(PIOC, LED2_PIO_IDX_MASK);
		pio_clear(PIOB, LED3_PIO_IDX_MASK);
		gfx_mono_draw_string("    PAUSE    ", 0,16, &sysfont);
	}
	// caso a nota seja uma pausa
	if (freq == 0) {
		gfx_mono_draw_string("             ", 0,16, &sysfont);
		pio_clear(PIOD, BUZZ_PIO_IDX_MASK);
		// pausa pelo tempo inserido
		delay_ms(time);
	}
	else {
		char snum[5];
		itoa(freq, snum, 10);
		gfx_mono_draw_string("             ", 10,50, &sysfont);
		gfx_mono_draw_string(snum, 50,16, &sysfont);
		if(freq<350){pio_clear(PIOA, LED1_PIO_IDX_MASK);}
		else if(freq<550){pio_clear(PIOC, LED2_PIO_IDX_MASK);}
		else{pio_clear(PIOB, LED3_PIO_IDX_MASK);}
		
		// demarca quanto tempo demora pra uma onda completar 1 ciclo
		int delay = 1000000/freq;
		// repete x ciclos na quantidade de tempo inserida
		for(int i=0; i<(1000*time/delay); i++){
			pio_set(PIOD, BUZZ_PIO_IDX_MASK);
			delay_us(delay/2);
			pio_clear(PIOD, BUZZ_PIO_IDX_MASK);
			delay_us(delay/2);
		}
		
		pio_set(PIOA, LED1_PIO_IDX_MASK);
		pio_set(PIOC, LED2_PIO_IDX_MASK);
		pio_set(PIOB, LED3_PIO_IDX_MASK);
		
		
	}
}

void back_song(disc* disc1) {
	if (contador==0) {
		contador = CONT_MAX;
		} else {
		contador -= 1;
	}
	disc1->curr_song = disc1->songs[contador];
}

void next_song(disc* disc1) {
	if (contador==CONT_MAX) {
		contador = 0;
		} else {
		contador += 1;
	}
	disc1->curr_song = disc1->songs[contador];
}

void play(song curr_song) {
	// c�digo retirado do reposit�rio onde se encontram as m�sicas
	
	// copia a melodia da musica atual pra variavel melody
	int melody_size = curr_song.size;
	int melody[melody_size];
	memcpy(melody, curr_song.melody, melody_size);
	int tempo = curr_song.tempo;
	
	int notes = sizeof(melody) / sizeof(melody[0]) / 2;
	int wholenote = (60000 * 4) / tempo;
	int divider = 0, noteDuration = 0;
	
	for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
		divider = melody[thisNote + 1];
		if (divider > 0) {
			noteDuration = (wholenote) / divider;
			} else if (divider < 0) {
			noteDuration = (wholenote) / abs(divider);
			noteDuration *= 1.5;
		}

		// toca a nota
		tone(melody[thisNote], noteDuration * 0.9);
		// breve pausa para poder diferenciar entre notas
		delay_ms(noteDuration*0.1);
				
			}
		}

		


		/************************************************************************/
		/* Main                                                                 */
		/************************************************************************/

		// Funcao principal chamada na inicalizacao do uC.
		int main(void)
		{
			sysclk_init();
			board_init();
			init();
			delay_init();
			gfx_mono_ssd1306_init();
			gfx_mono_draw_string("APS 1", 50,16, &sysfont);
			delay_ms(1000);
			
			
			song got, mii, mario, starwars;

			strcpy( got.title, got_title);
			got.tempo = got_tempo;
			got.melody = &got_melody;
			got.size = sizeof(got_melody)/sizeof(got_melody[0]);

			strcpy( mii.title, mii_title);
			mii.tempo = mii_tempo;
			mii.melody = &mii_melody;
			mii.size = sizeof(mii_melody)/sizeof(mii_melody[0]);

			strcpy( mario.title, mario_title);
			mario.tempo = mario_tempo;
			mario.melody = &mario_melody;
			mario.size = sizeof(mario_melody)/sizeof(mario_melody[0]);
			
			strcpy( starwars.title, starwars_title);
			starwars.tempo = starwars_tempo;
			starwars.melody = &starwars_melody;
			starwars.size = sizeof(starwars_melody)/sizeof(starwars_melody[0]);

			disc disc1;
			disc1.songs[0]= got;
			disc1.songs[1]= mii;
			disc1.songs[2]= mario;
			disc1.songs[3]= starwars;
			disc1.curr_song = disc1.songs[contador];
			
			while (1){
				//-------------------------------------------------------------------------------
				gfx_mono_draw_string(disc1.curr_song.title, 0,16, &sysfont);
				pio_set(PIOC, LED_PIO_IDX_MASK);
				pio_set(PIOA, LED1_PIO_IDX_MASK);
				pio_set(PIOC, LED2_PIO_IDX_MASK);
				pio_set(PIOB, LED3_PIO_IDX_MASK);
					
				//-------------------------------------------------------------------------------
				if (!pio_get(PIOD, PIO_INPUT, BUT1_PIO_IDX_MASK)){
					back_song(&disc1);
					gfx_mono_draw_string("    BACK     ", 0,16, &sysfont);
					delay_ms(500);
					pio_clear(PIOA, LED1_PIO_IDX_MASK);
					delay_ms(500);
				}

				//-------------------------------------------------------------------------------
				if (!pio_get(PIOC, PIO_INPUT, BUT2_PIO_IDX_MASK)){
					pio_clear(PIOC, LED2_PIO_IDX_MASK);
					gfx_mono_draw_string("    PLAY     ", 0,16, &sysfont);
					delay_ms(500);
					pio_clear(PIOC, LED2_PIO_IDX_MASK);
					delay_ms(500);
					gfx_mono_draw_string("             ", 0,16, &sysfont);
					play(disc1.curr_song);
				}
					
				//-------------------------------------------------------------------------------
				if (!pio_get(PIOA, PIO_INPUT, BUT3_PIO_IDX_MASK)){
					next_song(&disc1);
					gfx_mono_draw_string("    NEXT     ", 0,16, &sysfont);
					delay_ms(500);
					pio_clear(PIOB, LED3_PIO_IDX_MASK);
					delay_ms(500);
				}
			}
			return 0;
		}