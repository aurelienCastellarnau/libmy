/*
** main.c for demoLib in /home/aurelien/Documents/demoLib/libmy_extended
** 
** Made by CASTELLARNAU Aurelien
** Login   <castel_a@etna-alternance.net>
** 
** Started on  Wed Apr  5 11:44:31 2017 CASTELLARNAU Aurelien
** Last update Sat Jul  1 00:33:46 2017 CASTELLARNAU Aurelien
*/

#include <stdlib.h>
#include "libmy.h"

void    display_help()
{
  my_putstr("\nUse: (! = mandatory)\n");
  my_putstr("\nOptional: -v [level] / with level = debug|info|warning|error\n");
  my_putstr("\nOptional: -ipconfig [address]! [port]!\n");
  my_putstr("\nOptional: -h: display usage\n");
}

// For an example with a int as expected parameter.
void		fake_ipconfig(t_chain *parameters)
{
  int		port;

  my_putstr("\nThe address is set to: ");
  my_putstr((char*)((t_link*)parameters->dictionnary[0])->content);
  my_putstr("\nThe port is set to: ");

  // On cast le deuxième paramètre en int grâce à getnbr
  port = my_getnbr((char*)((t_link*)parameters->dictionnary[1])->content);
  // et on a un int...
  my_put_nbr(port);
  // de là on peut paramétrer
  my_putchar('\n');
}

void		display_hello(t_chain *parameters)
{
  t_link	*tmp;

  tmp = parameters->first;
  my_putstr("\n");
  while (tmp)
    {
      my_putstr((char*)tmp->content);
      my_putchar(' ');
      tmp = tmp->next;
    }
  /*
  ** Ou bien:
  ** my_putstr((char*)parameters->dictionnary[0]->content)
  ** Parce que l'on sait que le second paramètre doit être un int par exemple:
  ** my_put_nbr((int)parameters->dictionnary[1]->content)
  ** On peut ainsi affiner les paramètres, reste que le typage n'est pas pris en compte
  ** dans le process de parsing et que donc la gestion d'erreur à l'exécution de l'option
  ** doit être léché... si un int est attendu est que la chaine de caractères reçue n'est pas
  ** convertible par my_put_nbr, c'est le drame...
  */
  my_putstr("\n Hello World\n");
}

// LibList Etape 3
// A l'utilisateur de la lib de définir la manière d'exécuter
// ses options. Ici 2 façons, un simple display et une fonction
// qui prend une t_chain* (typiquement, les paramètres rangés de l'option)
int     exec(t_option *option)
{
  char  *opt;

  opt = option->name;
  my_putstr("\nExecution de la fonction de callback: ");
  my_putstr(opt);

  if (!my_strcmp(opt, "-h"))
    ((void (*)(void))option->action)();
  if (!my_strcmp(opt, "-v") || !my_strcmp(opt, "-ipconfig"))
    (*(void (*)(t_chain*))option->action)(option->parameters);
  return (0);
}

// display permet juste d'avoir l'affichage de __func__
// voir my_log() dans libmy/logger.c
void display(char *str, int lvl)
{
  my_log(__func__, str, lvl);
}

/*
** how to use libList, libParse and LibLog:
*/
int		main(int argc, char *argv[])
{
  /*
  ** LibParse fonctionne en corrélation avec libList
  ** Les fonctions validate_opt et check_mandatories attendent une t_chain en parametre
  ** voir libmy.h
  */
  
  // Etape 1, déclarer les options de type t_option
  // attendues par le programme au runtime:
  // Chainer ces options dans une t_chain
  int		act;
  t_chain	*chaine_options;
  t_option	*opt_log;
  t_option	*opt_conf;
  t_option	*opt_help;
  t_link	*ltmp;
  t_option	*otmp;

  act = 0;
  /*
  ** create_chain est conçue pour recevoir une fonction de free en paramètre
  ** si tel n'est pas le cas, lui passer NULL explicitement: create_chain(NULL);
  ** voir libmy/chain_manager.c
  */
  if ((chaine_options = create_chain(free_options_in_chain)) == NULL)
    return (1);

  // La fonction new_option(int, mandatory : l'option est-elle obligatoire? BOOL
  //                        int, mandatory_arguments : combien d'arguments obligatoires attend l'option? INT
  //                        int, allowed_arguments : combien d'arguments sont acceptés par l'option? INT
  //                        char*, opt: nom de l'option attendue "-help" ou "-port" par exemple
  //                        void*, action: la fonction a exécuter si l'option est présente.
  //                           )

  // LIBLOG => build_logger instancie le logger en static,
  // de là, la fonction my_log() est utilisable.
  // Si l'option -v n'est pas passée au programme, my_log ne fait rien
  if ((opt_log = new_option(0, 0, 2, "-v", build_logger)) == NULL)
    return (1);

  if ((opt_conf = new_option(0, 2, 0, "-ipconfig", fake_ipconfig)) == NULL)
    return (1);
  if ((opt_help = new_option(0, 0, 0, "-h", display_help)) == NULL)
    return (1);

  // On active les chaines de parametres pour les options qui doivent en avoir
  // La t_chain opt_log->parameters ne contient que des string dans ses links
  // donc pas besoin de fonction de free particulière, c'est prévu dans delete_chain.
  if ((opt_log->parameters = create_chain(NULL)) == NULL)
    return (1);
  if ((opt_conf->parameters = create_chain(NULL)) == NULL)
    return (1);

  // on link les options
  if (add_link(&chaine_options, opt_log))
    return (1);
  if (add_link(&chaine_options, opt_conf))
    return (1);
  if (add_link(&chaine_options, opt_help))
    return (1);

  // Etape 2: Une fois les options définies (typiquement, dans une fonction get_options)
  // on passe argv et argc à une fonction parse().
  if (parse(1, argc, argv, &chaine_options))
    display_help();
  else
    {
      // chaine_options est maintenant checkée (option obligatoires, paramètres obligatoires),
      // les options embarquent leur jeux de paramètres rangé dans une t_chain à option->parameters
      // de là, il reste à définir une fonction qui exécute, c'est l'étape 3 => voir exec()
      ltmp = chaine_options->first;
      while (ltmp)
	{
	  otmp = ((t_option*)ltmp->content);
	  // to_execute est un boolean setté durant le parsing
	  // exec(t_option*) est à définir par l'utilisateur
	  if (otmp->to_execute)
	    {
	      act = 1;
	      exec(otmp);
	    }
	  ltmp = ltmp->next;
	}
    }

  // pas zapper...
  delete_chain(&chaine_options);
  // revient à faire:
  // free_options_in_chain(chaine_options);
  // delete_chain(chaine_options);
  // puisque dans chaine_option->free il y a notre fonction de libération.
  
  // LIBLOG display appelle my_log après parsing des arguments,
  // un appel sur my_log sans appel de build_logger au préalable
  // doit entraîner du segfault...
  display("This is a fake debug message!", 4);
  display("This is a fake info!", 3);
  display("This is a fake warning!", 2);
  display("This is a fake error!", 1);
  if (!act)
    my_putstr_color("red", "\nrerun with -h\n");
  delete_logger();
  return (0);
}
