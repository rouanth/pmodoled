#include "pmodoled.h"
#include <string.h>

#define TRY(op) { int retcode = op; if (retcode) return retcode; }

#define CHECK_DO(op, ...) return self->op ? \
	self->op(self->data, ##__VA_ARGS__) : 0;

static int set_pin_value (struct pmodoled_drv *self, enum pin pin, bool set)
{
    CHECK_DO(set_pin_value, pin, set);
}

static int write_spi_byte (struct pmodoled_drv *self, uint8_t byte)
{
    CHECK_DO(write_spi_byte, byte);
}

static int wait_ms (struct pmodoled_drv *self, uint32_t duration)
{
    CHECK_DO(wait_ms, duration);
}

void pmodoled_status_init(struct pmodoled_status *status)
{
    memset(status, 0x0, sizeof(struct pmodoled_status));
}

int pmodoled_dry_run(struct pmodoled_drv *self, void *data,
                     int fun(struct pmodoled_drv *, void *data))
{
#define DRY_RUN_SAVE(op) void *op = self->op; self->op = NULL;
#define DRY_RUN_RESTORE(op) self->op = op;
    DRY_RUN_SAVE(write_spi_byte);
    DRY_RUN_SAVE(set_pin_value);
    DRY_RUN_SAVE(wait_ms);

    int retcode = fun(self, data);

    DRY_RUN_RESTORE(wait_ms);
    DRY_RUN_RESTORE(set_pin_value);
    DRY_RUN_RESTORE(write_spi_byte);
#undef DRY_RUN_SAVE
#undef DRY_RUN_RESTORE

    return retcode;
}

int pmodoled_turn_on (struct pmodoled_drv *self)
{
    TRY(set_pin_value(self, PIN_DC,    0));
    TRY(set_pin_value(self, PIN_VDDC,  0));
    TRY(wait_ms(self, 1));

    TRY(write_spi_byte(self, 0xAE));

    TRY(set_pin_value(self, PIN_VBATC, 0));
    TRY(wait_ms(self, 100));

    TRY(write_spi_byte(self, 0xAF));

    pmodoled_status_init(&self->status);
    self->status.enabled = true;

    return 0;
}

int pmodoled_turn_off (struct  pmodoled_drv *self)
{
    self->status.enabled = false;

    TRY(write_spi_byte(self, 0xaE));

    TRY(set_pin_value(self, PIN_VBATC, 1));
    TRY(wait_ms(self, 100));

    TRY(set_pin_value(self, PIN_VDDC,  1));

    return 0;
}

