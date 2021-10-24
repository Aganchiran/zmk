/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_pad_code

#include <device.h>
#include <drivers/behavior.h>
#include <logging/log.h>

#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/behavior.h>

#include <zmk/keys.h>
#include <stdlib.h>

#define DIGITS_IN_UINT32_T 10

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static void getDigits(uint32_t number, uint32_t out_digits[]) {
    uint32_t lastDigit = UINT32_MAX;
    uint32_t digits[DIGITS_IN_UINT32_T] = { 0 }; // max digits in uint32_t
    uint32_t iteration = 9;
    uint32_t numberOfDigits = 0;

    if (number != 0)
    {
        while (number > 0)
        {
            lastDigit = number % 10;

            digits[iteration--] = lastDigit; // first access the array and then substract

            number /= 10;
            numberOfDigits++;
        }
    }
    else
    {
        numberOfDigits = 1;
    }

    memcpy(out_digits, digits + (DIGITS_IN_UINT32_T - numberOfDigits), 
            numberOfDigits * sizeof(uint32_t));
}

static uint32_t getKeycode(uint32_t number){
    switch (number)
    {
    case 0:
        return KP_N0;

    case 1:
        return KP_N1;

    case 2:
        return KP_N2;

    case 3:
        return KP_N3;

    case 4:
        return KP_N4;

    case 5:
        return KP_N5;

    case 6:
        return KP_N6;

    case 7:
        return KP_N7;

    case 8:
        return KP_N8;

    case 9:
        return KP_N9;

    default:
        return 0;
    }
}


static int behavior_pad_code_init(const struct device *dev) { return 0; };

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    LOG_DBG("position %d keycode 0x%02X", event.position, binding->param1);

    uint32_t digits[DIGITS_IN_UINT32_T] = { 0 }; // max digits in uint32_t
    getDigits(binding->param2, digits);

    if (binding->param1 == 0) {
        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(LALT, true, event.timestamp));

        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(KP_N0, true, event.timestamp));
        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(KP_N0, false, event.timestamp));

        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(getKeycode(digits[0]), true, event.timestamp));
        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(getKeycode(digits[0]), false, event.timestamp));
        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(getKeycode(digits[1]), true, event.timestamp));
        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(getKeycode(digits[1]), false, event.timestamp));
        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(getKeycode(digits[2]), true, event.timestamp));
        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(getKeycode(digits[2]), false, event.timestamp));
        
        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(LALT, false, event.timestamp));
    } else {
        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(LALT, true, event.timestamp));

        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(getKeycode(digits[0]), true, event.timestamp));
        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(getKeycode(digits[0]), false, event.timestamp));
        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(getKeycode(digits[1]), true, event.timestamp));
        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(getKeycode(digits[1]), false, event.timestamp));
        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(getKeycode(digits[2]), true, event.timestamp));
        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(getKeycode(digits[2]), false, event.timestamp));
        
        ZMK_EVENT_RAISE(
            zmk_keycode_state_changed_from_encoded(LALT, false, event.timestamp));
    }

    

    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    LOG_DBG("position %d keycode 0x%02X", event.position, binding->param1);

    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_pad_code_driver_api = {
    .binding_pressed = on_keymap_binding_pressed, 
    .binding_released = on_keymap_binding_released
    };

#define KP_INST(n)                                                                                 \
    DEVICE_AND_API_INIT(behavior_pad_code_##n, DT_INST_LABEL(n), behavior_pad_code_init, NULL,   \
                        NULL, APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                    \
                        &behavior_pad_code_driver_api);

DT_INST_FOREACH_STATUS_OKAY(KP_INST)
