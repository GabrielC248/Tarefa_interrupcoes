#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "ws2812.pio.h"

// ---------------- Variáveis - INÍCIO ----------------

static volatile uint32_t last_time = 0; // Variável para armazenar o tempo do último callback.
static volatile int contador = 0;       // Contador dos números.
static volatile bool controle = false;  // Controle para o WS2812.

// ---------------- Variáveis - FIM ----------------

// ---------------- Defines - INÍCIO ----------------

#define green_button 5 // Define o pino do botão verde.
#define red_button 6   // Define o pino do botão vermelho.

#define red_rgb 13     // Define o pino vermelho do LED.
#define green_rgb 11   // Define o pino verde do LED.
#define blue_rgb 12    // Define o pino azul do LED.

#define luz 1        // Define a intensidade da luz (0-255)

// ---------------- Defines - FIM ----------------

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

// Função para facilitar o desenho no WS2812 utilizando 3 matrizes para o R, G e B.
void npDraw(uint8_t vetorR[5][5], uint8_t vetorG[5][5], uint8_t vetorB[5][5])
{
  int i, j,idx,col;
    for (i = 0; i < 5; i++) {
        idx = (4 - i) * 5; // Calcula o índice base para a linha.
        for (j = 0; j < 5; j++) {
            col = (i % 2 == 0) ? (4 - j) : j; // Inverte a ordem das colunas nas linhas pares.
            npSetLED(idx + col, vetorR[i][j], vetorG[i][j], vetorB[i][j]); // Preenche o buffer com os valores da matriz.
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
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
  };
  npDraw(vetorRG,vetorRG,vetorB); // Carrega os buffers.
  npWrite();                      // Escreve na matriz de LEDS.
  npClear();                      // Limpa os buffers (não necessário, mas por garantia).
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
    {  0  ,  0  , luz ,  0  ,  0  },
    {  0  , luz , luz ,  0  ,  0  },
    {  0  ,  0  , luz ,  0  ,  0  },
    {  0  ,  0  , luz ,  0  ,  0  },
    {  0  , luz , luz , luz ,  0  }
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
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  ,  0  ,  0  },
    {  0  , luz , luz , luz ,  0  }
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
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
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
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  }
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
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  ,  0  ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
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
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  ,  0  ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
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
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  }
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
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  }
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
    {  0  , luz , luz , luz ,  0  },
    {  0  , luz ,  0  , luz ,  0  },
    {  0  , luz , luz , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  },
    {  0  ,  0  ,  0  , luz ,  0  }
  };
  npDraw(vetorRG, vetorRG, vetorB);
  npWrite();
  npClear();
}

// Função para para exibir o número passado como parâmetro na matriz de LEDS.
int handle_numbers(int num) {
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
    default:
      return 1;
      break;
  }
  printf("%d Escrito!\n", num); // Print para visualização no terminal.
  return 0;
}

// ---------------- Números - FIM ----------------

// ---------------- Inicializações e Interrupção - INÍCIO ----------------

// Inicializa os botões configurando os pinos apropriados.
void init_buttons() {
  gpio_init(red_button);
  gpio_init(green_button);
  gpio_set_dir(red_button, GPIO_IN);
  gpio_set_dir(green_button, GPIO_IN);
  gpio_pull_up(red_button);
  gpio_pull_up(green_button);
}

// Inicializa o LED RGB configurando os pinos apropriados.
void init_RGB() {
  gpio_init(red_rgb);
  gpio_init(green_rgb);
  gpio_init(blue_rgb);
  gpio_set_dir(red_rgb,GPIO_OUT);
  gpio_set_dir(green_rgb,GPIO_OUT);
  gpio_set_dir(blue_rgb,GPIO_OUT);
  gpio_put(red_rgb, 0);
  gpio_put(green_rgb, 0);
  gpio_put(blue_rgb, 0);
}

// Callback da interrupção dos botões.
void gpio_irq_callback(uint gpio, uint32_t events) {
  // Obtém o tempo atual em microssegundos.
  uint32_t current_time = to_ms_since_boot(get_absolute_time());

  // Verifica se passou tempo suficiente desde o último evento.
  if (current_time - last_time > 200) { // 200 ms de debouncing.
    last_time = current_time; // Atualiza o tempo do último evento.
    if( (gpio == red_button) && (contador > 0) ) {
      contador--; // Decrementa o contador se o botão vermelho for pressionado.
    }else
    if( (gpio == green_button) && (contador < 9) ) {
      contador++; // Incrementa o contador se o botão verde for pressionado.
    }
    controle = true; // Sinaliza que houve uma mudança no contador
    printf("Interrupt! GPIO = %d | Contador = %d\n", gpio, contador); // Print para visualização no terminal.
  }
}

// ---------------- Inicializações e Interrupção - FIM ----------------

int main()
{
  float contagem = 0; // Contador de piscadas do LED.

  stdio_init_all();

  // Inicializa matriz de LEDs NeoPixel.
  npInit(LED_PIN);
  npClear();
  num_0();

  // Inicializa os botões.
  init_buttons();

  // Inicializa o LED RGB.
  init_RGB();

  // Configuração da interrupção.
  gpio_set_irq_enabled_with_callback(red_button, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_callback);
  gpio_set_irq_enabled_with_callback(green_button, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_callback);

  while (true)
  {
    sleep_ms(100); // Aguarda 100 ms para piscar o LED 5 vezes por segundo e também garantir a estabilidade do simulador.

    if(controle) {
      handle_numbers(contador); // Atualiza a exibição do número se houve alteração no contador.
      controle = false; // Reseta a flag de controle.
    }

    gpio_put(red_rgb, !gpio_get(red_rgb)); // Alterna o estado do LED vermelho.
    contagem++; // Incrementa a contagem de piscadas.
    printf("Contagem de piscadas: %.2f\n",contagem/2); // Exibe a contagem de piscadas.
  }
}