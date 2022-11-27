#include "mygpiod.h"
#include <gpiod.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#ifndef CONSUMER
#define CONSUMER  "Consumer"
#endif

IOMatrixA20::IOMatrixA20(int tms, int tck, int tdi, int tdo)
 : TMSPin(tms), TCKPin(tck), TDIPin(tdi), TDOPin(tdo) //, 1("1"), 0("0")
{
  //chipname = "gpiochip0";
  chip = gpiod_chip_open_by_name(chipname);
  if (!chip) {
    std::cerr << "ERROR: Open chip failed " << std::endl;
  }
  line_tdi = gpiod_chip_get_line(chip, TDIPin);
  if (!line_tdi) {
    std::cerr << "ERROR: Get line tdi failed " << std::endl;
  }
  line_tdo = gpiod_chip_get_line(chip, TDOPin);
  if (!line_tdo) {
    std::cerr << "ERROR: Get line tdo failed " << std::endl;
  }
  line_tck = gpiod_chip_get_line(chip, TCKPin);
  if (!line_tck) {
    std::cerr << "ERROR: Get line tck failed " << std::endl;
  }
  line_tms = gpiod_chip_get_line(chip, TMSPin);
  if (!line_tms) {
    std::cerr << "ERROR: Get line tms failed " << std::endl;
  }
  gpiod_line_request_output(line_tdi, CONSUMER, 0);
  gpiod_line_request_output(line_tms, CONSUMER, 0);
  gpiod_line_request_output(line_tck, CONSUMER, 0);
  gpiod_line_request_input (line_tdo, CONSUMER);
}

IOMatrixA20::~IOMatrixA20() {
  gpiod_line_release(line_tdi);
  gpiod_line_release(line_tdo);
  gpiod_line_release(line_tms);
  gpiod_line_release(line_tck);
  gpiod_chip_close(chip);
}

void IOMatrixA20::txrx_block(const unsigned char *tdi, unsigned char *tdo,
                             int length, bool last) {
  int i = 0;
  int j = 0;
  unsigned char tdo_byte = 0;
  unsigned char tdi_byte;

  if (tdi) tdi_byte = tdi[j];

  while (i < length - 1) {
    tdo_byte = tdo_byte + (txrx(false, (tdi_byte & 1) == 1) << (i % 8));
    if (tdi) tdi_byte = tdi_byte >> 1;
    i++;
    if ((i % 8) == 0) {            // Next byte
      if (tdo) tdo[j] = tdo_byte;  // Save the TDO byte
      tdo_byte = 0;
      j++;
      if (tdi) tdi_byte = tdi[j];  // Get the next TDI byte
    }
  };
  tdo_byte = tdo_byte + (txrx(last, (tdi_byte & 1) == 1) << (i % 8));
  if (tdo) tdo[j] = tdo_byte;

  gpiod_line_set_value(line_tck, 0);

  return;
}

void IOMatrixA20::tx_tms(unsigned char *pat, int length, int force) {
  int i;
  unsigned char tms;
  for (i = 0; i < length; i++) {
    if ((i & 0x7) == 0) tms = pat[i >> 3];
    tx((tms & 0x01), true);
    tms = tms >> 1;
  }

  gpiod_line_set_value(line_tck, 0);
}

void IOMatrixA20::tx(bool tms, bool tdi) {
  gpiod_line_set_value(line_tck, 0);

  gpiod_line_set_value(line_tdi, tdi ? 1 : 0);

  gpiod_line_set_value(line_tms, tms ? 1 : 0);

  gpiod_line_set_value(line_tck, 1);
}

bool IOMatrixA20::txrx(bool tms, bool tdi) {

  tx(tms, tdi);
  return gpiod_line_get_value(line_tdo);
}
