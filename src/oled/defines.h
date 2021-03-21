// LED verde da placa
#define LED_PIO           PIOC
#define LED_PIO_ID        ID_PIOC
#define LED_PIO_IDX       8
#define LED_PIO_IDX_MASK  (1 << LED_PIO_IDX)

// Botão da placa SW300
#define BUT_PIO			  PIOA
#define BUT_PIO_ID		  ID_PIOA
#define BUT_PIO_IDX		  11
#define BUT_PIO_IDX_MASK  (1u << BUT_PIO_IDX)

// Periféricos do OLED1
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

// Buzzer conectado direto à placa
#define BUZZ_PIO			PIOD
#define BUZZ_PIO_ID		ID_PIOD
#define BUZZ_PIO_IDX		30
#define BUZZ_PIO_IDX_MASK (1 << BUZZ_PIO_IDX)