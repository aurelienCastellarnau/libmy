/*
** arg.h for libmy_extended in /home/aurelien/Documents/Lib_etna/castel_a/libmy_extended/include
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Thu Feb  2 00:59:14 2017 CASTELLARNAU Aurelien
** Last update Wed Feb  8 22:31:52 2017 CASTELLARNAU Aurelien
*/

#ifndef _ARG_H_
# define _ARG_H_

void		display_help(void);
void		display_hello(void);
t_chain		*get_options();
int		execute_argv(int argc, char *argv[]);

#endif /* !_ARG_H_ */
