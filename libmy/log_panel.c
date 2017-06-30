/*
** log_panel.c for libmy_extended in /home/aurelien/Documents/demoLib/demoParse/libmy
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Wed Apr 12 23:35:39 2017 CASTELLARNAU Aurelien
** Last update Mon Apr 17 21:07:41 2017 CASTELLARNAU Aurelien
*/

#include "libmy.h"

void    log_error(char *str)
{
  my_putstr_color("red", "< ERROR > ");
  my_putstr(str);
}

void    log_warning(char *str)
{
  my_putstr_color("yellow", "< WARNING > ");
  my_putstr(str);
}

void    log_info(char *str)
{
  my_putstr_color("blue", "< INFO > ");
  my_putstr(str);
}

void    log_debug(char *str)
{
  my_putstr_color("green", "< DEBUG > ");
  my_putstr(str);
}
