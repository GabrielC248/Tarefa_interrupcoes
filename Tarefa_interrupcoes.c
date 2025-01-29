#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "pico/cyw43_arch.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"
#include "ws2812.pio.h"

// ---------------- DEFINES - INÍCIO ----------------

#define green_button 5
#define red_button 6

#define rgb_red 11
#define rgb_green 12
#define rgb_blue 13

// ---------------- DEFINES - FIM ----------------

// ---------------- WS2812 - INÍCIO ----------------

// Definição do número de LEDs e pino.
#define LED_COUNT 25
#define LED_PIN 7

// Definição de pixel GRB
struct pixel_t
{
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

// Inicializa a máquina PIO para controle da matriz de LEDs.
void npInit(uint pin)
{

  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2812_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0)
  {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2812_program_init(np_pio, sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i)
  {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

// Atribui uma cor RGB a um LED.
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b)
{
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

// Limpa o buffer de pixels.
void npClear()
{
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}

// Escreve os dados do buffer nos LEDs.
void npWrite()
{
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i)
  {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

// ---------------- WS2812 - FIM ----------------

// ---------------- Desenhar - INÍCIO ----------------

void npDraw(uint8_t vetorR[5][5], uint8_t vetorG[5][5], uint8_t vetorB[5][5])
{
  int i, j,idx,col;
    for (i = 0; i < 5; i++) {
        idx = (4 - i) * 5; // Calcula o índice base para a linha
        for (j = 0; j < 5; j++) {
            col = (i % 2 == 0) ? (4 - j) : j; // Inverte a ordem das colunas nas linhas pares
            npSetLED(idx + col, vetorR[i][j], vetorG[i][j], vetorB[i][j]);
        }
    }
}

// ---------------- Desenhar - FIM ----------------

// ---------------- Números - INÍCIO ----------------

void num_0() {
  uint8_t vetorRG[5][5] = {
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  }
  };
  uint8_t vetorB[5][5] = {
    {  0  , 255 , 255 , 255 ,  0  },
    {  0  , 255 ,  0  , 255 ,  0  },
    {  0  , 255 ,  0  , 255 ,  0  },
    {  0  , 255 ,  0  , 255 ,  0  },
    {  0  , 255 , 255 , 255 ,  0  }
  };
  npDraw(vetorRG,vetorRG,vetorB);
  npWrite();
  npClear();
}

void num_1() {
  uint8_t vetorRG[5][5] = {
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  }
  };
  uint8_t vetorB[5][5] = {
    {  0  ,  0  , 255 ,  0  ,  0  },
    {  0  , 255 , 255 ,  0  ,  0  },
    {  0  ,  0  , 255 ,  0  ,  0  },
    {  0  ,  0  , 255 ,  0  ,  0  },
    {  0  , 255 , 255 , 255 ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

void num_2() {
  uint8_t vetorRG[5][5] = {
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  }
  };
  uint8_t vetorB[5][5] = {
    {  0  , 255 , 255 , 255 ,  0  },
    {  0  ,  0  ,  0  , 255 ,  0  },
    {  0  , 255 , 255 , 255 ,  0  },
    {  0  , 255 ,  0  ,  0  ,  0  },
    {  0  , 255 , 255 , 255 ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

void num_3() {
  uint8_t vetorRG[5][5] = {
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  }
  };
  uint8_t vetorB[5][5] = {
    {  0  , 255 , 255 , 255 ,  0  },
    {  0  ,  0  ,  0  , 255 ,  0  },
    {  0  , 255 , 255 , 255 ,  0  },
    {  0  ,  0  ,  0  , 255 ,  0  },
    {  0  , 255 , 255 , 255 ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

void num_4() {
  uint8_t vetorRG[5][5] = {
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  }
  };
  uint8_t vetorB[5][5] = {
    {  0  , 255 ,  0  , 255 ,  0  },
    {  0  , 255 ,  0  , 255 ,  0  },
    {  0  , 255 , 255 , 255 ,  0  },
    {  0  ,  0  ,  0  , 255 ,  0  },
    {  0  ,  0  ,  0  , 255 ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

void num_5() {
  uint8_t vetorRG[5][5] = {
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  }
  };
  uint8_t vetorB[5][5] = {
    {  0  , 255 , 255 , 255 ,  0  },
    {  0  , 255 ,  0  ,  0  ,  0  },
    {  0  , 255 , 255 , 255 ,  0  },
    {  0  ,  0  ,  0  , 255 ,  0  },
    {  0  , 255 , 255 , 255 ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

void num_6() {
  uint8_t vetorRG[5][5] = {
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  }
  };
  uint8_t vetorB[5][5] = {
    {  0  , 255 , 255 , 255 ,  0  },
    {  0  , 255 ,  0  ,  0  ,  0  },
    {  0  , 255 , 255 , 255 ,  0  },
    {  0  , 255 ,  0  , 255 ,  0  },
    {  0  , 255 , 255 , 255 ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

void num_7() {
  uint8_t vetorRG[5][5] = {
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  }
  };
  uint8_t vetorB[5][5] = {
    {  0  , 255 , 255 , 255 ,  0  },
    {  0  ,  0  ,  0  , 255 ,  0  },
    {  0  ,  0  ,  0  , 255 ,  0  },
    {  0  ,  0  ,  0  , 255 ,  0  },
    {  0  ,  0  ,  0  , 255 ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

void num_8() {
  uint8_t vetorRG[5][5] = {
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  }
  };
  uint8_t vetorB[5][5] = {
    {  0  , 255 , 255 , 255 ,  0  },
    {  0  , 255 ,  0  , 255 ,  0  },
    {  0  , 255 , 255 , 255 ,  0  },
    {  0  , 255 ,  0  , 255 ,  0  },
    {  0  , 255 , 255 , 255 ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

void num_9() {
  uint8_t vetorRG[5][5] = {
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  },
    {  0  ,  0  ,  0  ,  0  ,  0  }
  };
  uint8_t vetorB[5][5] = {
    {  0  , 255 , 255 , 255 ,  0  },
    {  0  , 255 ,  0  , 255 ,  0  },
    {  0  , 255 , 255 , 255 ,  0  },
    {  0  ,  0  ,  0  , 255 ,  0  },
    {  0  ,  0  ,  0  , 255 ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

// ---------------- Números - FIM ----------------

void handle_numbers(int num) {
  switch(num) {
    case 0:
      num_0();
      break;
    case 1:
      num_1();
      break;
    case 2:
      num_2();
      break;
    case 3:
      num_3();
      break;
    case 4:
      num_4();
      break;
    case 5:
      num_5();
      break;
    case 6:
      num_6();
      break;
    case 7:
      num_7();
      break;
    case 8:
      num_8();
      break;
    case 9:
      num_9();
      break;
  }
}

void handle_keypress(char key)
{
  switch (key)
  {
  case '1':
    //animacao1();
    break;
  case '*':
    printf("Entrando no modo BOOTSEL. Aguarde...\n");
    sleep_ms(500);        // Pequena pausa para garantir a saída do texto
    reset_usb_boot(0, 0); // Reinicia no modo BOOTSEL
    break;
  default:
    break;
  }
}

void init_buttons() {
  gpio_init(red_button);
  gpio_init(green_button);
  gpio_set_dir(red_button, GPIO_IN);
  gpio_set_dir(green_button, GPIO_IN);
  gpio_pull_up(red_button);
  gpio_pull_up(green_button);
}

void init_RGB() {
  gpio_init(rgb_red);
  gpio_init(rgb_green);
  gpio_init(rgb_blue);
  gpio_set_dir(rgb_red,GPIO_OUT);
  gpio_set_dir(rgb_green,GPIO_OUT);
  gpio_set_dir(rgb_blue,GPIO_OUT);
  gpio_put(rgb_red, 0);
  gpio_put(rgb_green, 0);
  gpio_put(rgb_blue, 0);
}

int main()
{
  int contador = 0, change_control = 1;

  stdio_init_all();

  // Inicializa matriz de LEDs NeoPixel.
  npInit(LED_PIN);
  npClear();

  // Inicializa os botões
  init_buttons();

  // Inicializa os botões
  init_RGB();

  while (true)
  {
    sleep_ms(20); // Aguarda 20 milissegundos para melhor funcionamento do simulador
    
    if(!gpio_get(red_button)) {
      if(contador > 0) {
        contador--;
        change_control = 1;
      }
    }else
    if(!gpio_get(green_button)) {
      if(contador < 9) {
        contador++;
        change_control = 1;
      }
    }
    if(change_control) {
      handle_numbers(contador);
      change_control = 0;
      sleep_ms(180);
    }
  }
}