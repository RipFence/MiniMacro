#pragma once

enum Buttn
{
  RD,
  GN,
  BE,
  YW,
  BK,
  WHT,
  BUTTNCOUNT
};
extern const int buttnPins[];
extern const char *buttnNames[];

void hwSetup();
void hwLoop();
