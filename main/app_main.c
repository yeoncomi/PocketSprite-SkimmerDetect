#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>

#include "8bkc-hal.h"
#include "ugui.h"
#include "8bkc-ugui.h"
#include "powerbtn_menu.h"
#include "sndmixer.h"
#include "esp_bt.h"

extern const uint8_t beep_wav[] asm("_binary_beep_wav");
static void do_powerbtn_menu() {
    int i=powerbtn_menu_show(tilegfx_get_fb());
    if (i==POWERBTN_MENU_EXIT) kchal_exit_to_chooser();
    if (i==POWERBTN_MENU_POWERDOWN)
    {
        esp_bluedroid_disable();
        esp_bluedroid_deinit();
        kchal_power_down();
    }
}

int do_menu() {
    if (btn&KC_BTN_POWER) do_powerbtn_menu();
}

int bluetooth() {
    esp_bt_controller_enable();
    esp_bluedroid_init();
    esp_bluedroid_enable();
    bluetooth_get = esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY,0x05,0);

}

int sound() {
    sndmixer_quee_wav(beep_wav);
    sndmixer_play();
}

void display(const char* text) {
    Kcugui_cls();
    UG_FontSelect(&FONT_6X8);
    UG_SetForecolor(C_WHITE);
    UG_PutString(0, 0, text);
    kcugui_flush();
}

int main() {
    kchal_init();
    sndmixer_init(2,22050)
    kcgui_cls();
    bluetooth();
    while (1) {
        int r = do_menu();
        if (r == MENU_OPT_START) {
            kchal_exit_to_chooser();
        }
        if (bluetooth_get == "ESP_FAIL"){
            display("Bluetooth Discovery Failed")
        }
        if (bluetooth_get == "ESP_OK")
            display("Bluetooth Discovery Suceeded")
        
    }
}

