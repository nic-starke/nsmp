/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
#pragma once
/* -------------------------------- Includes -------------------------------- */

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <inttypes.h>
#include <unistd.h>

#include "nsmp_linux.h"

/* -------------------------------- Defines --------------------------------- */
/* -------------------------------- Externs --------------------------------- */
/* -------------------------------- Enums ----------------------------------- */
/* -------------------------------- Structs --------------------------------- */
/* -------------------------------- Declarations ---------------------------- */

static int f_init(void);
static int f_open(void);
static int f_close(void);
static int f_transmit(uint8_t* buffer, size_t buf_len, size_t num_tx);
static int f_receive(uint8_t* buffer, size_t buf_len, size_t* num_rx);

static int validate_baudrate(int baudrate);

/* -------------------------------- Globals --------------------------------- */
/* -------------------------------- Locals ---------------------------------- */

static nsmp_netif_drv_s drv = {
		.init			= f_init,
		.open			= f_open,
		.close		= f_close,
		.transmit = f_transmit,
};

nsmp_netif_s nsmp_netif_linux_termios = {
		.id		= 0,
		.drv	= &drv,
		.ctx	= &ctx,
		.next = NULL,
};

static struct termios tty = {
		.c_cflag = CS8 | CLOCAL |
							 CREAD, /* 8-bit chars, ignore modem control, enable reading */
		.c_iflag		 = IGNPAR, /* ignore parity errors */
		.c_oflag		 = 0,			 /* no output flags */
		.c_lflag		 = 0,			 /* no local flags */
		.c_cc[VMIN]	 = 1,			 /* block until 1 byte is received */
		.c_cc[VTIME] = 0,			 /* no timeout */
};

static int				 s_fd	 = -1;
static const char* s_dev = NULL;
static int				 s_baudrate;

/* -------------------------------- Functions ------------------------------- */

int nsmp_termios_configure(const char* dev, int baudrate) {
	s_dev			 = dev;
	s_baudrate = baudrate;
	return NSMP_OK;
}

static f_init(void) {
	RETURN_IF_NULL(s_dev);
	return validate_baudrate(s_baudrate);
}

static f_open(void) {
	s_fd = open(s_dev, O_RDWR | O_NOCTTY | O_NONBLOCK);

	if (s_fd < 0) {
		return NSMP_ERR_BAD_FILEDESC;
	}

	int ret = tcgetattr(s_fd, &tty);

	if (ret != 0) {
		printf("Error from tcgetattr: %s\n", strerror(ret));
		return NSMP_ERR_EXTERNAL;
	}

	ret = validate_baudrate(s_baudrate);

	if (ret != NSMP_OK) {
		printf("Invalid baudrate: %d\n", s_baudrate);
		return ret;
	}

	cfsetispeed(&tty, s_baudrate);
	cfsetospeed(&tty, s_baudrate);
	tcsetattr(s_fd, TCSANOW, &tty);

	return NSMP_OK;
}

static f_close(void) {
	if (s_fd >= 0) {
		close(s_fd);
	}

	return NSMP_OK;
}

static int f_transmit(uint8_t* buffer, size_t num_tx) {
	RETURN_IF_NULL(buffer);
	RETURN_ERR_IF_FALSE(num_tx <= 0, NSMP_ERR_BAD_ARG);
	ssize_t count = 0;

	do {
		int ret = write(s_fd, buffer + count, num_tx - count);
		if (ret == -1) {
			return NSMP_ERR_TRANSMIT;
		} else {
			count += (ssize_t)ret;
		}

	} while (count < num_tx);

	return NSMP_OK;
}

static int f_receive(uint8_t* buffer, size_t buf_len, size_t* num_rx) {
	RETURN_IF_NULL(buffer);
	RETURN_IF_NULL(num_rx);
	RETURN_ERR_IF_FALSE(buf_len <= 0, NSMP_ERR_BAD_ARG);

	ssize_t ret = read(s_fd, buffer, buf_len);

	if (ret == -1) {
		return NSMP_ERR_RECEIVE;
	}

	*num_rx = (size_t)ret;

	return NSMP_OK;
}

static int validate_baudrate(int baudrate) {
	switch (baudrate) {
		case 9600:
		case 19200:
		case 38400:
		case 57600:
		case 115200:
		case 230400:
		case 460800:
		case 500000:
		case 576000:
		case 921600: return NSMP_OK;
		default: return NSMP_ERR_BAD_BAUD;
	}
}
