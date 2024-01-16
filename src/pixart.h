#pragma once

/**
 * @file pixart.h
 *
 * @brief Common header file for all optical motion sensor by PIXART
 */

#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

#ifdef __cplusplus
extern "C" {
#endif

enum pixart_input_mode { MOVE = 0, SCROLL, SNIPE };

/* device data structure */
struct pixart_data {
    const struct device *dev;

    enum pixart_input_mode curr_mode;
    uint32_t curr_cpi;
    int32_t scroll_delta;

#ifdef CONFIG_PMW3320_POLLING_RATE_125_SW
    int64_t last_poll_time;
    int16_t last_x;
    int16_t last_y;
#endif

    // motion interrupt isr
    struct gpio_callback irq_gpio_cb;
    // the work structure holding the trigger job
    struct k_work trigger_work;

    // the work structure for delayable init steps
    struct k_work_delayable init_work;
    int async_init_step;

    //
    bool ready;           // whether init is finished successfully
    bool last_read_burst; // todo: needed?
    int err;              // error code during async init

    // for pmw3320 smart algorithm
    bool sw_smart_flag;
};

// device config data structure
struct pixart_config {
    struct gpio_dt_spec irq_gpio;
    struct spi_dt_spec bus;
    struct gpio_dt_spec cs_gpio;
    size_t scroll_layers_len;
    int32_t scroll_layers[10];
    size_t snipe_layers_len;
    int32_t snipe_layers[];
};

/** @brief Sensor specific attributes of PIXART. */
enum pixart_attribute {
    /** Sensor CPI for both X and Y axes. */
    PIXART_ATTR_CPI = SENSOR_ATTR_PRIV_START,

    /** Sensor CPI for X and Y axes. */
    PIXART_ATTR_XCPI,
    PIXART_ATTR_YCPI,

    /** Enable or disable sleep modes. */
    PIXART_ATTR_REST_ENABLE,

    /** Entering time from Run mode to REST1 mode [ms]. */
    PIXART_ATTR_RUN_DOWNSHIFT_TIME,

    /** Entering time from REST1 mode to REST2 mode [ms]. */
    PIXART_ATTR_REST1_DOWNSHIFT_TIME,

    /** Entering time from REST2 mode to REST3 mode [ms]. */
    PIXART_ATTR_REST2_DOWNSHIFT_TIME,

    /** Sampling frequency time during REST1 mode [ms]. */
    PIXART_ATTR_REST1_SAMPLE_TIME,

    /** Sampling frequency time during REST2 mode [ms]. */
    PIXART_ATTR_REST2_SAMPLE_TIME,

    /** Sampling frequency time during REST3 mode [ms]. */
    PIXART_ATTR_REST3_SAMPLE_TIME,

    /** Select the running mode. */
    PIXART_ATTR_RUN_MODE,
};

#ifdef __cplusplus
}
#endif

/**
 * @}
 */
