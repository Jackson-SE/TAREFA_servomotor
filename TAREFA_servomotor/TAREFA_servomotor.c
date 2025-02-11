#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22

// Configuração da frequência PWM (50Hz)
void set_pwm_frequency(uint slice_num) {
    // Para 50Hz, um divisor de clock de aproximadamente 156.25 (125MHz / (50Hz * 4096))
    // garante um período de 20ms (1/50Hz).
    pwm_set_clkdiv(slice_num, 156.25f);
}

// Função para definir a largura do pulso em microssegundos
void set_servo_pulse_width(uint slice_num, uint pulse_width_us) {
    // O valor do nível PWM é proporcional à largura do pulso em microssegundos.
    // O período total do PWM é de 20ms (50Hz).
    // O valor máximo do PWM (4095) corresponde a 20ms.
    // Portanto, a relação é: nível_pwm = (largura_pulso_us / 20000) * 4095
    uint level = (uint)((float)pulse_width_us / 20000.0f * 4095.0f);
    pwm_set_gpio_level(SERVO_PIN, level);
}

int main() {
    stdio_init_all();

    // Configura o pino GPIO como saída PWM
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);

    // Define a frequência do PWM para 50Hz
    set_pwm_frequency(slice_num);

    // Inicializa o PWM
    pwm_start(slice_num);

    while (1) {
        // Posição 180 graus (2400µs)
        printf("Posicionando em 180 graus\n");
        set_servo_pulse_width(slice_num, 2400);
        sleep_ms(5000);

        // Posição 90 graus (1470µs)
        printf("Posicionando em 90 graus\n");
        set_servo_pulse_width(slice_num, 1470);
        sleep_ms(5000);

        // Posição 0 graus (500µs)
        printf("Posicionando em 0 graus\n");
        set_servo_pulse_width(slice_num, 500);
        sleep_ms(5000);

        // Rotação periódica suave (0 a 180 graus)
        printf("Rotação periódica suave\n");
        for (int pulse_width = 500; pulse_width <= 2400; pulse_width += 10) {
            set_servo_pulse_width(slice_num, pulse_width);
            sleep_ms(20); // Ajuste a velocidade da rotação
        }
        for (int pulse_width = 2400; pulse_width >= 500; pulse_width -= 10) {
            set_servo_pulse_width(slice_num, pulse_width);
            sleep_ms(20); // Ajuste a velocidade da rotação
        }
    }

    return 0;
}