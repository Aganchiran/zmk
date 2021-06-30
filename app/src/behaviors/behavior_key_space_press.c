/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_key_space_press

#include <device.h>
#include <drivers/behavior.h>
#include <logging/log.h>

#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/behavior.h>

#include <zmk/matrix.h>
#include <zmk/endpoints.h>
#include <zmk/events/modifiers_state_changed.h>
#include <zmk/hid.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static int behavior_key_space_press_init(const struct device *dev) { return 0; };

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    LOG_DBG("position %d keycode 0x%02X", event.position, binding->param1);

    const struct device* keyPressBinding = device_get_binding("KEY_PRESS");

    char bindingName[strlen(keyPressBinding->name)];
    strcpy(bindingName, keyPressBinding->name); // Copy to avoid const change
    
    struct zmk_behavior_binding keyBinding = { bindingName, binding->param1, binding->param2 }; // Char[] is a pointer

    behavior_keymap_binding_pressed(&keyBinding, event);
    behavior_keymap_binding_released(&keyBinding, event);
    behavior_keymap_binding_pressed(&keyBinding, event);
    behavior_keymap_binding_released(&keyBinding, event);

    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    LOG_DBG("position %d keycode 0x%02X", event.position, binding->param1);

    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_key_space_press_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released
};

#define KP_INST(n)                                                                                 \
    DEVICE_AND_API_INIT(behavior_key_space_press_##n, DT_INST_LABEL(n), behavior_key_space_press_init, NULL,   \
                        NULL, APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                    \
                        &behavior_key_space_press_driver_api);

DT_INST_FOREACH_STATUS_OKAY(KP_INST)
