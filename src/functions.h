#include "defines.h"
#include "structs.h"

volatile int pause_flag = 1;
int contador = 1;



void tone(int freq, int time, int progress) {
	
	while(pause_flag==1){
		pio_clear(PIOA, LED1_PIO_IDX_MASK);
		pio_clear(PIOC, LED2_PIO_IDX_MASK);
		pio_clear(PIOB, LED3_PIO_IDX_MASK);
		gfx_mono_draw_string("    PAUSE    ", 0,16, &sysfont);
	}
	char str[13];
	char snum[5];
	itoa(progress, snum, 10);
	memset(str, 'o', progress);
	char str1[14];
	char str2[14];
	strcpy (str1," ");
	strcpy (str2,"ooooooooooooo");
	strncat (str1, str2, progress);
	gfx_mono_draw_string("             ", 0,16, &sysfont);
	gfx_mono_draw_string(str1, 0,16, &sysfont);
	
	
	//gfx_mono_draw_string(snum, 50,16, &sysfont);
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



void play(song curr_song) {
	// código retirado do repositório onde se encontram as músicas
	
	// copia a melodia da musica atual pra variavel melody
	int progress = 0;
	int melody_size = curr_song.size;
	int tempo = curr_song.tempo;
	
	int notes = melody_size /2;
	int wholenote = (60000 * 4) / tempo;
	int divider = 0, noteDuration = 0;
	
	for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
		
		progress = (int)thisNote/(notes/13)/2;
		
		divider = curr_song.melody[thisNote + 1];
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
		tone(curr_song.melody[thisNote], noteDuration * 0.9, progress);
		// breve pausa para poder diferenciar entre notas
		delay_ms(noteDuration*0.1);
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