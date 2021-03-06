#include "defines.h"
#include "main.c"

void tone(int freq, int time) {
	while(pause_flag==1){
		pio_clear(PIOA, LED1_PIO_IDX_MASK);
		pio_clear(PIOC, LED2_PIO_IDX_MASK);
		pio_clear(PIOB, LED3_PIO_IDX_MASK);
		gfx_mono_draw_string("    PAUSE    ", 0,16, &sysfont);
	}
	gfx_mono_draw_string("             ", 0,16, &sysfont);
	gfx_mono_draw_string("    PLAY     ", 0,16, &sysfont);
	// caso a nota seja uma pausa
	if (freq == 0) {
		pio_clear(PIOD, BUZZ_PIO_IDX_MASK);
		// pausa pelo tempo inserido
		delay_ms(time);
	}
	else {
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
	// c?digo retirado do reposit?rio onde se encontram as m?sicas
	
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
		if (!pio_get(PIOA, PIO_INPUT, BUT3_PIO_IDX_MASK)){
			thisNote = notes*2;
			gfx_mono_draw_string("    STOP     ", 0,16, &sysfont);
			delay_ms(500);
			pio_clear(PIOB, LED3_PIO_IDX_MASK);
			delay_ms(500);
		}
		
		// toca a nota
		tone(melody[thisNote], noteDuration * 0.9);
		// breve pausa para poder diferenciar entre notas
		delay_ms(noteDuration*0.1);
		
	}
}