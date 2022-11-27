#ifndef __IO_MYGPIOD__
#define __IO_MYGPIOD__

#include "iobase.h"

class IOMatrixA20 : public IOBase {
 public:
  IOMatrixA20(int tms, int tck, int tdi, int tdo);
  virtual ~IOMatrixA20();

 private:
  void tx(bool tms, bool tdi);
  bool txrx(bool tms, bool tdi);

  void txrx_block(const unsigned char *tdi, unsigned char *tdo, int length,
                  bool last);
  void tx_tms(unsigned char *pat, int length, int force);

  int open_write_close(const char *name, const char *valstr);
  int setup_gpio(int gpio, int is_input);
  void unexport_gpio(int gpio);
  bool is_gpio_valid(int gpio) { return gpio >= 0 && gpio < 1000; }

 private:
  int TMSPin;
  int TCKPin;
  int TDIPin;
  int TDOPin;

  char *chipname = "gpiochip0";
  struct gpiod_chip *chip;
  struct gpiod_line *line_tdi;
  struct gpiod_line *line_tdo;
  struct gpiod_line *line_tms;
  struct gpiod_line *line_tck;

  //const char *one;
  //const char *zero;
};
#endif
