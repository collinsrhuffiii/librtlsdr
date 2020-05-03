#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "rtl-sdr.h"
#include "convenience/convenience.h"

static rtlsdr_dev_t *dev = NULL;

#define DEFAULT_BUF_LENGTH		(16 * 16384)

static void rtlsdr_callback(unsigned char *buf, uint32_t len, void *ctx)
{
}

int async_fn() {
    int r;

    r = rtlsdr_read_async(dev, rtlsdr_callback, NULL,
                  1, DEFAULT_BUF_LENGTH);

    printf("async_fn rtlsdr_read_async r = %d\n", r);

    return r;
}

int main() {
	int dev_index = 0;
    int r;
    pthread_t async_thread;

    dev_index = verbose_device_search("0");
	r = rtlsdr_open(&dev, (uint32_t)dev_index);

    r = pthread_create(&async_thread, NULL, (void *)async_fn, NULL);

    sleep(1);
    printf("Cancelling async read\n");
    rtlsdr_cancel_async(dev);

    printf("Joining async thread\n");
    pthread_join(async_thread, NULL);

	rtlsdr_close(dev);
}
