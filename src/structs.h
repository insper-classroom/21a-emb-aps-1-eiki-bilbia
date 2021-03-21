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