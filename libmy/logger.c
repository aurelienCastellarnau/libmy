/*
** logger.c for libmy_extended in /home/aurelien/Documents/Lib_etna/castel_a/libmy_extended/libmy
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Tue Feb  7 23:01:39 2017 CASTELLARNAU Aurelien
** Last update Mon Apr 24 22:04:13 2017 CASTELLARNAU Aurelien
*/

#include <stdlib.h>
#include "libmy.h"

const static void_char log_panel[] = {
  NULL,
  log_error,
  log_warning,
  log_info,
  log_debug
};

int		set_level(t_logger **logger, char *level)
{
  if (!my_strcmp(level, "debug"))
    {
      (*logger)->lvl = 4;
      (*logger)->level = my_strdup("DEBUG");
    }
  if (!my_strcmp(level, "info"))
    {
      (*logger)->lvl = 3;
      (*logger)->level = my_strdup("INFO");
    }
  if (!my_strcmp(level, "warning"))
    {
      (*logger)->lvl = 2;
      (*logger)->level = my_strdup("WARNING");
    }
  if (!my_strcmp(level, "error"))
    {
      (*logger)->lvl = 1;
      (*logger)->level = my_strdup("ERROR");
    }
  return (0);
}

int		set_parameters(t_logger **logger, t_chain *parameters)
{
  t_link	*ltmp;
  char		*content;
  
  ltmp = parameters->first;
  if (ltmp == NULL)
    {
      (*logger)->lvl = 1;
      (*logger)->level = my_strdup("OFF");
    }
  while (ltmp)
    {
      if ((*logger)->level == NULL
	  && set_level(logger, (char*)ltmp->content))
	{
	  devlog(__func__, "\nlog can't be set to", 1);
	  devlog(__func__, (char*)ltmp->content, 1);
	  ltmp = NULL;
	}
      else
	ltmp = ltmp->next;
    }
  return (0);
}

t_logger		*build_default_logger()
{
  t_logger		*logger;
  
  if ((logger = malloc(sizeof(*logger))) == NULL)
    return (NULL);
  logger->level = my_strdup("DEBUG");
  logger->log = my_log;
  logger->lvl = 4;
  return (logger);
}

t_logger		*get_default_logger()
{
  t_logger		*logger;
  
  if ((logger = build_default_logger()) == NULL || logger->level == NULL)
    return (NULL);
  return (logger);
}

void		devlog(const char *func, char *str, int lvl)
{
 t_logger	*logger;

  if ((logger = get_default_logger()) != NULL)
    {
      if (lvl <= logger->lvl)
	{
	  my_putstr("\n[CS/");
	  my_putstr(func);
	  my_putstr("] ");
	  log_panel[lvl](str);
	}
      free(logger->level);
      free(logger);
    }
}

void		my_log(const char *func, char *str, int lvl)
{
  t_logger	*logger;

  if ((logger = get_logger()) != NULL)
    {
      if (lvl <= logger->lvl)
	{
	  my_putstr("\n[CS/");
	  my_putstr(func);
	  my_putstr("] ");
	  log_panel[lvl](str);
	}
    }
}

t_logger		*build_logger(t_chain *parameters)
{
  static t_logger	*logger;
  
  if (logger == NULL && parameters != NULL)
    {

      if ((logger = malloc(sizeof(*logger))) == NULL)
	return (NULL);
      logger->level = NULL;
      logger->log = my_log;
      if (parameters == NULL)
	{
	  logger->level = my_strdup("OFF");
	  logger->lvl = 0;
	}
      else
	{
	  if (set_parameters(&logger, parameters))
	    return (NULL);
	}
    }
  return (logger);
}

t_logger		*get_logger()
{
  t_logger		*logger;
  
  if ((logger = build_logger(NULL)) == NULL || logger->level == NULL)
    return (NULL);
  return (logger);
}

void		delete_logger()
{
  t_logger	*logger;

  if ((logger = get_logger()) != NULL)
    {
      if (logger->level != NULL)
	free(logger->level);
      free(logger);
    }
}
