/************************************************************************/
/* APS1 - Buzzer Musical                                                */
/* Beatriz Muniz de Castro e Silva                                      */
/* Eiki Luis Yamashiro                                                  */
/*                                                                      */
/* 5 semestre - Eng. da Computação - Insper                             */
/* Rafael Corsi - rafael.corsi@insper.edu.br                            */
/*                                                                      */
/* Músicas adquiridas no repositório:                                   */
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
#include "functions.h"


/************************************************************************/
/* defines                                                              */
/************************************************************************/

// As frequências das notas foram guardadas no arquivo notas_freq.h
// Os periféricos foram definidps no arquivo defines.h


/************************************************************************/
/* músicas                                                              */
/************************************************************************/

#include "GoT.h"
#include "Mii.h"
#include "Mario.h"
#include "StarWars.h"


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

// Funções de reprodução no arquivo functions.h


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