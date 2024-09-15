#include "bsp_api.h"  // Thư viện BSP API cho SK-S7G2

#define LED_COUNT 3  // Số lượng đèn LED
#define BUTTON1_PORT IOPORT_PORT_00_PIN_06   // Chân cho Button 1 (SW1)
#define BUTTON2_PORT IOPORT_PORT_00_PIN_05   // Chân cho Button 2 (SW2)

// Cấu trúc để chứa thông tin về các đèn LED
bsp_leds_t leds;

// Hàm delay
void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < (ms * 6000); i++) {
        __asm("NOP");  // Lệnh NOP cho delay
    }
}

// Cấu hình các chân nút bấm làm đầu vào
void configure_buttons_as_input() {
    R_IOPORT_PinCfg(BUTTON1_PORT, IOPORT_CFG_PORT_DIRECTION_INPUT | IOPORT_CFG_PULLUP_ENABLE);  // Kéo lên cho Button 1
    R_IOPORT_PinCfg(BUTTON2_PORT, IOPORT_CFG_PORT_DIRECTION_INPUT | IOPORT_CFG_PULLUP_ENABLE);  // Kéo lên cho Button 2
}

// Hàm chính
void hal_entry(void) {
    R_BSP_LedsGet(&leds);  // Lấy thông tin về các chân LED từ BSP

    configure_buttons_as_input();  // Cấu hình nút bấm thành input mode

    // Kiểm tra nếu có LED trên bo mạch
    if (leds.led_count > 0) {
        while (1) {
            // Đọc trạng thái của Button 1 (SW1)
            ioport_level_t button1_state;
            R_IOPORT_PinRead(BUTTON1_PORT, &button1_state);

            // Đọc trạng thái của Button 2 (SW2)
            ioport_level_t button2_state;
            R_IOPORT_PinRead(BUTTON2_PORT, &button2_state);

            // Nếu Button 1 được nhấn (LOW), các LED sáng lần lượt
            if (button1_state == IOPORT_LEVEL_LOW) {
                while(1){
                    R_IOPORT_PinRead(BUTTON1_PORT, &button1_state);
                    if(button1_state == IOPORT_LEVEL_HIGH){
                        break;
                    }
                    for (int i = 0; i < LED_COUNT; i++) {
                        R_IOPORT_PinWrite(leds.p_leds[i], IOPORT_LEVEL_LOW);  // Bật LED
                        delay_ms(1000);
                        R_IOPORT_PinWrite(leds.p_leds[i], IOPORT_LEVEL_HIGH); // Tắt LED
                    }
                }

            }

            // Nếu Button 2 được nhấn (LOW), tất cả LED chớp tắt
            if (button2_state == IOPORT_LEVEL_LOW) {
                while(1){
                    R_IOPORT_PinRead(BUTTON2_PORT, &button2_state);
                    if(button2_state == IOPORT_LEVEL_HIGH){
                        break;
                    }
                    for (int i = 0; i < LED_COUNT; i++) {
                        R_IOPORT_PinWrite(leds.p_leds[i], IOPORT_LEVEL_LOW);  // Bật LED
                    }
                    delay_ms(1000);
                    for (int i = 0; i < LED_COUNT; i++) {
                        R_IOPORT_PinWrite(leds.p_leds[i], IOPORT_LEVEL_HIGH); // Tắt LED
                    }
                    delay_ms(1000);
                }
            }

            delay_ms(100);  // Delay nhỏ để tránh xử lý quá nhanh
        }
    }
}
