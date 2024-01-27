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

static int validate_baudrate(int baudrate);

/* -------------------------------- Globals --------------------------------- */
/* -------------------------------- Locals ---------------------------------- */

static struct termios tty = {
		.c_cflag = CS8 | CLOCAL |
							 CREAD, /* 8-bit chars, ignore modem control, enable reading */
		.c_iflag		 = IGNPAR, /* ignore parity errors */
		.c_oflag		 = 0,			 /* no output flags */
		.c_lflag		 = 0,			 /* no local flags */
		.c_cc[VMIN]	 = 1,			 /* block until 1 byte is received */
		.c_cc[VTIME] = 0,			 /* no timeout */
};

/* -------------------------------- Functions ------------------------------- */

int nsmp_linux_netif_init(nsmp_linux_netif_ctx_s* ctx) {
	RETURN_IF_NULL(ctx);
	RETURN_IF_NULL(ctx->dev);
	RETURN_IF_TRUE(ctx->state != NSMP_PEER_STATE_UNINITIALIZED, NSMP_ERR_BAD_SEQ);

	int ret = validate_baudrate(ctx->baud);
	RETURN_IF_ERR(ret);

	ctx->fd = open(ctx->dev, O_RDWR | O_NOCTTY | O_NONBLOCK);

	if (ctx->fd < 0) {
		return NSMP_ERR_BAD_FILEDESC;
	}

	int ret = tcgetattr(ctx->fd, &tty);

	if (ret != 0) {
		printf("Error from tcgetattr: %s\n", strerror(ret));
		return NSMP_ERR_EXTERNAL;
	}

	if (ret != NSMP_OK) {
		printf("Invalid baudrate: %d\n", ctx->baud);
		return ret;
	}

	cfsetispeed(&tty, ctx->baud);
	cfsetospeed(&tty, ctx->baud);
	tcsetattr(ctx->fd, TCSANOW, &tty);

	ctx->state = NSMP_PEER_STATE_READY;

	return NSMP_OK;
}

int nsmp_linux_netif_stop(nsmp_linux_netif_ctx_s* ctx) {
	RETURN_IF_NULL(ctx);

	if (ctx->fd >= 0) {
		close(ctx->fd);
	}

	return NSMP_OK;
}

int nsmp_linux_netif_transmit(void* netif_ctx, uint8_t* buffer, size_t num_tx) {
	RETURN_IF_NULL(netif_ctx);
	RETURN_IF_NULL(buffer);
	RETURN_ERR_IF_TRUE(num_tx <= 0, NSMP_ERR_BAD_ARG);

	nsmp_linux_netif_ctx_s* ctx		= (nsmp_linux_netif_ctx_s*)netif_ctx;
	ssize_t									count = 0;

	do {
		int ret = write(ctx->fd, buffer + count, num_tx - count);
		if (ret == -1) {
			return NSMP_ERR_TRANSMIT;
		} else {
			count += (ssize_t)ret;
		}

	} while (count < num_tx);
	return NSMP_OK;
}

int nsmp_linux_netif_receive(nsmp_linux_netif_ctx_s* ctx, uint8_t* buffer,
														 size_t buf_len, size_t* num_rx) {
	RETURN_IF_NULL(ctx);
	RETURN_IF_NULL(buffer);
	RETURN_IF_NULL(num_rx);
	RETURN_ERR_IF_FALSE(buf_len <= 0, NSMP_ERR_BAD_ARG);
	ssize_t ret = read(ctx->fd, buffer, buf_len);

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
