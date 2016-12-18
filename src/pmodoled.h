#ifndef pmodoled_h_INCLUDED
#define pmodoled_h_INCLUDED

#define PMODOLED_WIDTH  128
#define PMODOLED_HEIGHT 32

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

enum pin {
    PIN_DC,
    PIN_RES,
    PIN_VBATC,
    PIN_VDDC,
};

struct pmodoled_status {
    bool inverted;
    uint8_t framebuffer[PMODOLED_HEIGHT][PMODOLED_WIDTH];
};

struct pmodoled_drv {
    int  (*write_spi_byte)(void *, uint8_t);
    int  (*set_pin_value) (void *, enum pin, bool);
    void *data;

    struct pmodoled_status pmodoled_status;
};

int pmodoled_status_init(struct pmodoled_status_init *);

int pmodoled_dry_run(struct pmodoled_drv *, void *data,
                     int fun(struct pmodoled_drv *, void *data));

int pmodoled_turn_on (struct pmodoled_drv *);
int pmodoled_turn_off(struct pmodoled_drv *);

#endif // pmodoled_h_INCLUDED
