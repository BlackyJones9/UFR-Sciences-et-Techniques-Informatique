//  op_ip.h : précisions sur l'implantation du module op.

//-------------------------------------------------------------------------------------------

//  La structure d'options est de taille constante.
//  La taille du champ aref de la structure de contexte varie mais elle est 
//    toujours égale à une puissance de 2.

//  Lorsqu'ils ne sont pas constants, les couts sont exprimés en fonction de la
//    valeur de certains des arguments de la fonction.
//-------------------------------------------------------------------------------------------

//  opts_empty : temps contant ; espace constant
//  opts_dispose : temps contant ; espace constant
//  opts_length_max : temps contant ; espace constant
//  opts_isPonct : temps contant ; espace constant
//  opts_isGraph : temps contant ; espace constant
//  opts_isAvl : temps contant ; espace constant

//  context_empty : temps contant ; espace constant
//  context_dispose : temps contant ; espace constant
//  context_entry : temps contant ; espace constant
//  context_pos : temps contant ; espace constant
//  context_aref : temps contant ; espace constant
//  is_std_file : temps contant ; espace constant

//  stdin_read_set : temps linéaire en argc ; 
//    espace au plus linéaire en la taille de (*lfiles)->aref.
//  print_files : temps linéaire en lfiles->cur_pos ; espace constant
//  print_help : temps linéaire en la taille de l'aide ; espace constant
