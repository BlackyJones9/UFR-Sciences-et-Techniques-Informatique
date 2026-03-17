%{
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>
  #include <limits.h>
  #include "../includes/type.h"
  #include "../includes/operand.h"
  #include "../includes/booleans.h"
  #include "../includes/hashtable/hashtable.h"
  #include "../includes/holdall/holdall.h"

  #define nullptr NULL
  #define END_CALL 0
  #define EXPR_CALL 1

  //Liste des variables
  typedef struct var_cell var_cell;
  struct var_cell{
    char *name;
    type_s type;
    int num;
    var_cell *v_infer_type;
    var_cell *next;
  };

  typedef struct {
    int entry;
    var_cell *head;
    var_cell *tail;
  } ctrl_var;

  //Cellule d'algo
  typedef struct {
    char *name;
    ctrl_var *ctrl_param;
    ctrl_var *ctrl_var_loc;
    type_s type;
    int param;
    int var_loc;
  } algo_cell;

  //Cellule call
  typedef struct call_cell call_cell;
  struct call_cell {
    algo_cell *algo;
    char *algo_name;
    algo_cell *cur_algo;
    ctrl_var *cv_cell;
    int used_in;
    type_s expected_type;
    call_cell *prev;
  };

  extern FILE *yyin;
  int yylex();
  void yyerror (char const *);
  int etiquette();
  size_t str_hashfun(const char *s);
  void assignType(type_s *p, type_s t);
  void loadVar(algo_cell *a_cell, var_cell *p_cell, bool isParam);
  void storeVar(algo_cell *a_cell, var_cell *p_cell);
  void storeReturnVal(algo_cell *a_cell);
  void storeContext();
  void restoreContext();
  var_cell *getVar_In_SymbolTable(char *ref);
  var_cell *var_cell_search(ctrl_var *ctrl, char *ref);
  void var_cell_add(ctrl_var *ctrl, char *ref, type_s type);
  void call_init_pile(char *ref);
  void call_restore_pile(char *ref);
  var_cell *get_param_cell_Xnum(algo_cell *cell, size_t num);

  hashtable *ght_algo;
  holdall *ghl_call;

  char *algo_name;
  call_cell *lastCall;
  bool isFirstParse;
%}

%code requires {
  typedef struct call_cell call_cell;
}

%union {
  int integer;
  char *string;
  type_s t;
  call_cell *call;
}
%type<t> EXPR
%type<t> LEXPR
%type<t> EXPR_CALL
%type<string> PARAM
%type<integer> TAG_INIT
%type<call> CALL_INIT

%token<integer> NUM
%token LE
%token GE
%token DIFF
%token TRUE
%token FALSE
%token AND
%token OR
%token XOR
%token NOT

%token B_ALGO
%token E_ALGO
%token<string> ID
%token RET
%token SET
%token FOR
%token WHILE
%token CALL
%token IF
%token ELSE
%token FI
%token OD

%left OR
%left XOR
%left AND
%left NOT
%left '<' '>' LE GE '=' DIFF
%left '+' '-'
%left '*' '/'
%left '(' ')'

//%nonassoc ALGO_END

%start PROG

%%

PROG:
    ALGO_LIST E_CALL
  | %empty
;

ALGO_LIST:
    ALGO_LIST ALGO
  | ALGO
;


E_CALL:
    {
      if (!isFirstParse) {
        printf(":main\n");
        printf("\tconst bp,stack\n");
        printf("\tconst sp,stack\n");
        printf("\tconst ax,2\n");
        printf("\tsub sp,ax\n");
      }
    }
  CALL_INIT '{' LVALUE '}'
    {
      if (!isFirstParse) {
        call_cell *c_cell = $2;
  
        algo_cell *a_cell = c_cell->algo;
        printf("\tconst ax,%s\n", c_cell->algo_name);
        printf("\tcall ax\n");
        
        int entry = a_cell->ctrl_param->entry +
                    a_cell->ctrl_var_loc->entry;
  
        for (int k = 0; k < entry; k++) {
          printf("\tpop ax\n");
        }
  
        printf("\tcp ax,sp\n");
        printf("\tcallprintfd ax\n");
        printf("\tpop ax\n");
        printf("\tend\n");
  
        printf(":stack\n");
        printf("@int 0\n");
      }
    }
;



EXPR_CALL:
  CALL_INIT '{' LVALUE '}'
  { if (isFirstParse) {
        $$ = UNDEF;
      } else {
        call_cell *c_cell = $1;

        hashtable_setCurrentCall(ght_algo, c_cell);

        algo_cell *a_cell = c_cell->algo;
        printf("\tconst ax,%s\n", c_cell->algo_name);
        printf("\tcall ax\n");

        $$ = a_cell == nullptr ? UNDEF : a_cell->type;

        int entry = a_cell->ctrl_param->entry +
                    a_cell->ctrl_var_loc->entry;

        for (int k = 0; k < entry; k++) {
          printf("\tpop ax\n");
        }
        call_cell *finished = $1;
        hashtable_setCurrentCall(ght_algo, finished->prev);
      }
    }
;

CALL_INIT:
  CALL '{' ID '}'
    {
      if (isFirstParse) {
        $$ = nullptr;
      } else {
        call_cell *c_cell = malloc(sizeof(*c_cell));
        if (!c_cell) {
          printf("not enough memory\n");
            exit(EXIT_FAILURE);
        }
        c_cell->prev = hashtable_getCurrentCall(ght_algo);
        hashtable_setCurrentCall(ght_algo, c_cell);
        lastCall = c_cell;
        
        algo_cell *target = hashtable_search(ght_algo, $3);
        c_cell->algo = target;
        c_cell->algo_name = $3;
        c_cell->expected_type = target->type;
        c_cell->used_in = EXPR_CALL;

        c_cell->cv_cell = calloc(1, sizeof(*(c_cell->cv_cell)));
        if (!c_cell->cv_cell) {
            printf("not enough memory\n");
            exit(EXIT_FAILURE);
        }
        holdall_put(ghl_call, c_cell);

        //Val de retour + var_loc
        printf("\tconst ax,0\n");
        algo_cell *a_cell = hashtable_search(ght_algo, $3);
        for (int k = 0; k < a_cell->ctrl_var_loc->entry + 1; k++) {
          printf("\tpush ax\n");
        }

        $$ = c_cell;
      }
    }
;

LVALUE:
    VALUE_LIST
  | %empty
;

VALUE_LIST:
    VALUE_LIST ',' VALUE
  | VALUE
;

VALUE:
  LEXPR
    {
      if (!isFirstParse) {
        //Récupère l'algo cible
        algo_cell *a_cell = hashtable_search(ght_algo, c_cell->algo_name);
        if (a_cell == nullptr) {
          printf("Algo inconnue\n");
          exit(EXIT_FAILURE);
        }

        var_cell *v_cell = malloc(sizeof(*v_cell));
        if (v_cell == nullptr) {
          printf("not enough memory\n");
          exit(EXIT_FAILURE);
        }
        call_cell *c_cell = hashtable_getCurrentCall(ght_algo);
        v_cell->type = $1;
        v_cell->num = c_cell->cv_cell->entry + 1;

        if ($1 == UNDEF) {
          var_cell *linked_var = get_var_cell_Xnum(c_cell->cur_algo, num); // n'existe pas
          if (linked_var == nullptr) {
            printf("variable inconnue au bataillon d'EXPLORATION\n");
            exit(EXIT_FAILURE);
          }
          v_cell->v_infer_type = linked_var;
        }
        
        v_cell->next = nullptr; 
        if (c_cell->cv_cell->head == nullptr) {
          c_cell->cv_cell->head = v_cell;
        } else {
          c_cell->cv_cell->tail->next = v_cell;
        }
        c_cell->cv_cell->tail = v_cell;
        c_cell->cv_cell->entry += 1;

        v_cell = get_param_cell_Xnum(a_cell, c_cell->cv_cell->entry);
        if (v_cell == nullptr) {
          printf("To many parameters in call\n");
          exit(EXIT_FAILURE);
        }
        if (v_cell->type == UNDEF) {
          v_cell->type = $1;
        } else if (v_cell->type != $1) {
          printf("Incompatible parameter type\n");
          exit(EXIT_FAILURE);
        }
      }
    }
;

ALGO_NAME_INIT:
  '{' ID '}'
    {
      algo_name = $2;

      //printf("ENTER %s\n", algo_name);
      algo_cell *a_cell = hashtable_search(ght_algo, algo_name);
      if (isFirstParse) {
        if (a_cell == NULL) {
          a_cell = malloc(sizeof(*a_cell));
          if (a_cell == NULL) {
            printf("not enough memory\n");
            return EXIT_FAILURE;
          }
          a_cell->name = algo_name;
          a_cell->type = UNDEF;
          a_cell->ctrl_param = malloc(sizeof(*(a_cell->ctrl_param)));
          a_cell->ctrl_var_loc = malloc(sizeof(*(a_cell->ctrl_var_loc)));
          if (a_cell->ctrl_param == nullptr || a_cell->ctrl_var_loc == nullptr) {
            printf("not enought memory\n");
            exit(EXIT_FAILURE);
          }
          a_cell->ctrl_param->entry = 0;
          a_cell->ctrl_param->head = nullptr;
          a_cell->ctrl_param->tail = nullptr;
          a_cell->ctrl_var_loc->entry = 0;
          a_cell->ctrl_var_loc->head = nullptr;
          a_cell->ctrl_var_loc->tail = nullptr;
          hashtable_add(ght_algo, algo_name, a_cell);
        } else {
          printf("La fonction '%s' est déjà définie\n", algo_name);
          exit(EXIT_FAILURE);
        }
      } else {
        a_cell->param = a_cell->ctrl_param->entry;
        //Déplace pointeur de pile
        printf(":%s\n", a_cell->name);
        printf("\tpush bp\n");
        printf("\tcp bp,sp\n");
        //Sauvegarde le context
        storeContext();
      }
      hashtable_setCurrentAlgo(ght_algo, a_cell);
    }
;


ALGO:
    B_ALGO ALGO_NAME_INIT PARAM_BLOCK LINSTR_BLOCK E_ALGO
    {
        if (isFirstParse) {
            algo_cell *a_cell = hashtable_getCurrentAlgo(ght_algo);
            a_cell->var_loc = a_cell->ctrl_var_loc->entry;
            hashtable_setCurrentCall(ght_algo, nullptr);
        }
    }
;

PARAM_BLOCK:
    '{' PARAM_LIST '}'
;

PARAM_LIST:
    LPARAM
  | %empty
;

LPARAM:
    LPARAM ',' PARAM
  | PARAM
;


PARAM:
  ID {
        if (isFirstParse) {
          algo_cell *a_cell = hashtable_getCurrentAlgo(ght_algo);
          var_cell *c = var_cell_search(a_cell->ctrl_param, $1);
          if (c != NULL) {
            printf("La variable '%s' existe déjà dans cet algo\n", $1);
            exit(EXIT_FAILURE);
          }
          var_cell_add(a_cell->ctrl_param, $1, UNDEF);
        }
      }
;

LEXPR:
    LEXPR EXPR
  | EXPR
;

LINSTR_BLOCK:
    LINSTR
  | %empty
;

LINSTR:
    LINSTR INSTR
  | INSTR
;

TAG_INIT:
  %empty 
    {
      if (!isFirstParse)
          $$ = etiquette();
      else
          $$ = 0;
    }
;

INSTR:
    RET '{' LEXPR '}'
      {
        algo_cell *a_cell = hashtable_getCurrentAlgo(ght_algo);
        type_s t = a_cell->type;
        if (t == UNDEF || t == $3) {
          a_cell->type = $3;
        } else {
          printf("2 return with different expression type\n");
          exit(EXIT_FAILURE);
        }

        if (!isFirstParse) {
          storeReturnVal(a_cell);
          restoreContext();
          printf("\tpop bp\n");
          printf("\tret\n");
        }
      }

  | SET '{' ID '}' '{' LEXPR '}'
      {
        algo_cell *a_cell = hashtable_getCurrentAlgo(ght_algo);
        var_cell *p_cell = var_cell_search(a_cell->ctrl_param, $3);

        if (p_cell == nullptr) {
          p_cell = var_cell_search(a_cell->ctrl_var_loc, $3);
          if (p_cell == nullptr) {
            //initialisation
            if (isFirstParse) {
              var_cell_add(a_cell->ctrl_var_loc, $3, $6);
            }
          } else {
            //update value
            if (p_cell->type != $6) {
              printf("Erreur: type différent\n");
              exit(EXIT_FAILURE);
            }
          }
          if (!isFirstParse) {
            storeVar(a_cell, p_cell);
          }
        } else {
          printf("Erreur: un paramètre possède déjà se nom\n");
          exit(EXIT_FAILURE);
        }
      }

  | FOR '{' ID '}' //DOFORI
      {
        if (isFirstParse) {
          var_cell *p_cell = getVar_In_SymbolTable($3);
          if (p_cell != nullptr) {
            printf("Erreur: variable déjà utilisée\n");
            exit(EXIT_FAILURE);
          }
          algo_cell *a_cell = hashtable_getCurrentAlgo(ght_algo);
          var_cell_add(a_cell->ctrl_var_loc, $3, INT_T);
        }
      }
    TAG_INIT
    '{' LEXPR '}' '{' LEXPR '}'
      {
        if (!isFirstParse) {

          if ($8 == UNDEF) {
            assignType(&$8, INT_T);
          }
          if ($11 == UNDEF) {
            assignType(&$11, INT_T);
          }
          if ($8 != INT_T || $11 != INT_T) {
            printf("Erreur: erreur de typage, expecting INT_T\n");
            exit(EXIT_FAILURE);
          }
          
          int e = $6;
          //début boucle
          printf(":dofori_%d\n", e);
          //store varLoc
          algo_cell *a_cell = hashtable_getCurrentAlgo(ght_algo);
          var_cell *p_cell = var_cell_search(a_cell->ctrl_var_loc ,$3);
          printf("\tpop cx\n");
          storeVar(a_cell, p_cell);
          printf("\tpush bx\n");
          printf("\tpush cx\n");
          //test
          printf("\tconst cx,do_%d\n", e);
          printf("\tpop ax\n");
          printf("\tpop bx\n");
          printf("\tsless bx,ax\n");
          printf("\tjmpc cx\n");
          printf("\tcmp ax,bx\n");
          printf("\tjmpc cx\n");
          //fin
          printf("\tconst cx,end_dofori_%d\n", e);
          printf("\tjmp cx\n");
          //exec linst
          printf(":do_%d\n", e);
          printf("\tpush bx\n");
          printf("\tpush ax\n");
        }
      }
    LINSTR OD
      {
        if (!isFirstParse) {
          int e = $6;
          //goto début
          printf("\tpop ax\n");
          printf("\tpop bx\n");
          printf("\tconst cx,1\n");
          printf("\tadd bx,cx\n");
          printf("\tpush bx\n");
          printf("\tpush ax\n");
          printf("\tconst cx,dofori_%d\n", e);
          printf("\tjmp cx\n");
          printf(":end_dofori_%d\n", e);
        }
      }

  | WHILE TAG_INIT
    {
      if (!isFirstParse) {
        int e = $2;
        // début boucle
        printf(":while_%d\n", e);
      }
    }
  '{' LEXPR '}' 
    {
      if (isFirstParse) {
          if ($2 != BOOL_T) {
            printf("Erreur: erreur de typage while, expecting BOOL_T\n");
            exit(EXIT_FAILURE);
          }
        } else {
          int e = $2;
          // L'étiquette while est au dessus pour que l'expression soit recalculée à chaque tour de boucle
          // On aura 0 (Faux) ou 1 (Vrai) en haut de la pile
          // On saute à end_while si on a faux
          printf("\tconst cx,end_while_%d\n", e);
          printf("\tpop ax\n");
          printf("\tconst bx,0\n");
          printf("\tcmp ax,bx\n");
          printf("\tjmpc cx\n");
        }
    }
   LINSTR OD
    {
      if (!isFirstParse) {
        int e = $2;
        //goto début
        printf("\tconst cx,while_%d\n", e);
        printf("\tjmp cx\n");
        printf(":end_while_%d\n", e);
      }
    }
    

  | IF TAG_INIT '{' LEXPR '}' 
    {
      if (isFirstParse) {
        if ($2 != BOOL_T) {
          printf("Erreur: erreur de typage IF, expecting BOOL_T\n");
          exit(EXIT_FAILURE);
        }
      } else {
        int e = $2;
        // On saute après les instructions dans le SI si on a faux
        printf("\tconst cx,end_if_%d\n", e);
        printf("\tpop ax\n");
        printf("\tconst bx,0\n");
        printf("\tcmp ax,bx\n");
        printf("\tjmpc cx\n");
      }
    }
  LINSTR 
    {
      if (!isFirstParse) {
        int e = $2;
        // Si on vient des instructions du SI on saute après l'éventuel else
        printf("\tconst cx,end_else_%d\n", e);
        printf("\tjmp cx\n");
        // Après cette étiquette soit on a rien soit on a les instructions dans le else
        printf(":end_if_%d\n", e);
      }
    }
  IF_STMT
    {
      if (!isFirstParse) {
        int e = $2;
        printf(":end_else_%d\n", e);
      }
    }
;

IF_STMT:
    FI
  | ELSE LINSTR FI
;


EXPR:
    EXPR_CALL

  | EXPR '+'
      {
        assignType(&$1, INT_T);
      }
    EXPR  
      {
        assignType(&$4, INT_T);
        if (bin_opr($1, $4, "add", isFirstParse) == INT_T) {
            $$=INT_T;
          } else {
            printf("Erreur de typage\n");
            exit(EXIT_FAILURE);
        }
      }

  | EXPR '-'
      {
        assignType(&$1, INT_T);
      }
    EXPR {
          assignType(&$4, INT_T);
          if (bin_opr($1, $4, "sub", isFirstParse) == INT_T) {
            $$=INT_T;
          } else {
            printf("Erreur de typage\n");
            exit(EXIT_FAILURE);
          }
        }

  | EXPR '*'
      {
        assignType(&$1, INT_T);
      }
    EXPR {
            assignType(&$4, INT_T);
            if (bin_opr($1, $4, "mul", isFirstParse) == INT_T) {
              $$=INT_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }

  | EXPR '/'
      {
        assignType(&$1, INT_T);
      }
    EXPR  {
            assignType(&$4, INT_T);
            if (div_opr($1, $4, isFirstParse) == INT_T) {
              $$=INT_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }

  | '(' EXPR ')' {$$=$2;}

  | NUM   {
            if (!isFirstParse) {
              printf("\tconst ax,%d\n", $1);
              printf("\tpush ax\n");
            }
            $$=INT_T;
          }

  | ID    {
            var_cell *p_cell;
            p_cell = getVar_In_SymbolTable($1);
            if (p_cell == nullptr) {
                printf("Erreur: variable %s inconue au bataillon\n", $1);
                exit(EXIT_FAILURE);
            }
            $$=p_cell->type;
            if (!isFirstParse) {
              algo_cell *a_cell = hashtable_getCurrentAlgo(ght_algo);
              var_cell *cell = var_cell_search(a_cell->ctrl_param, p_cell->name);
              if (cell == nullptr) {
                loadVar(a_cell, p_cell, false);
              } else {
                loadVar(a_cell, p_cell, true);
              }
            }
          }

  | TRUE  {
            if (!isFirstParse) {
              printf("\tconst ax,1\n");
              printf("\tpush ax\n");
            }
            $$=BOOL_T;
          }
  | FALSE {
            if (!isFirstParse) {
              printf("\tconst ax,0\n");
              printf("\tpush ax\n");
            }
            $$=BOOL_T;
          }
  | EXPR '<'
      {
        assignType(&$1, INT_T);
      }
    EXPR  {
            assignType(&$4, INT_T);
            if(inf_bool($1, $4, isFirstParse) == BOOL_T) {
              $$=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
  | EXPR '>'
      {
        assignType(&$1, INT_T);
      }
    EXPR  {
            assignType(&$4, INT_T);
            if(sup_bool($1, $4, isFirstParse) == BOOL_T) {
              $$=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
  | EXPR LE
      {
        assignType(&$1, INT_T);
      }
    EXPR {
            assignType(&$4, INT_T);
            if(inf_eq_bool($1, $4, isFirstParse) == BOOL_T) {
              $$=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
  | EXPR GE
      {
        assignType(&$1, INT_T);
      }
    EXPR {
            assignType(&$4, INT_T);
            if(sup_eq_bool($1, $4, isFirstParse) == BOOL_T) {
              $$=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
  | EXPR '='
      {
        assignType(&$1, INT_T);
      }
    EXPR  {
            assignType(&$4, INT_T);
            if(eq_bool($1, $4, isFirstParse) == BOOL_T) {
              $$=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
  | EXPR DIFF
      {
        assignType(&$1, INT_T);
      }
    EXPR  {
            assignType(&$4, INT_T);
            if(diff_bool($1, $4, isFirstParse) == BOOL_T) {
              $$=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
  | EXPR AND
      {
        assignType(&$1, BOOL_T);
      }
    EXPR  {
            assignType(&$4, BOOL_T);
            if($1==BOOL_T && $4==BOOL_T) {
              if (!isFirstParse) {
                printf("\tpop ax\n");
                printf("\tpop bx\n");
                printf("\tand ax,bx\n");
                printf("\tpush ax\n");
              }
              $$=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
  | EXPR OR
      {
        assignType(&$1, BOOL_T);
      }
    EXPR  {
            assignType(&$4, BOOL_T);
            if($1==BOOL_T && $4==BOOL_T) {
              if (!isFirstParse) {
                printf("\tpop ax\n");
                printf("\tpop bx\n");
                printf("\tor ax,bx\n");
                printf("\tpush ax\n");
              }
              $$=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
  | EXPR XOR
      {
        assignType(&$1, BOOL_T);
      }
    EXPR  {
            assignType(&$4, BOOL_T);
            if($1==BOOL_T && $4==BOOL_T) {
              if (!isFirstParse) {
                printf("\tpop ax\n");
                printf("\tpop bx\n");
                printf("\txor ax,bx\n");
                printf("\tpush ax\n");
              }
              $$=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
  | NOT EXPR  {
                assignType(&$2, BOOL_T);
                if($2==BOOL_T) {
                  if (!isFirstParse) {
                    printf("\tpop ax\n");
                    printf("\tnot ax\n");
                    printf("\tpush ax\n");
                  }
                  $$=BOOL_T;
                } else {
                  printf("Erreur de typage\n");
                  exit(EXIT_FAILURE);
                }
              }
;


%%

void yyerror (char const *s) {fprintf(stderr, "%s\n", s); exit(EXIT_FAILURE);}

var_cell *getVar_In_SymbolTable(char *ref) {
  algo_cell *a_cell = hashtable_getCurrentAlgo(ght_algo);
  var_cell *p_cell = var_cell_search(a_cell->ctrl_param, ref);
  //Cherche le paramètre dans TS
  if (p_cell != nullptr) {
    hashtable_setCurrentVar(ght_algo, p_cell);
    return p_cell;
  }

  //Cherche la Variable Local dans TS
  p_cell = var_cell_search(a_cell->ctrl_var_loc, ref);
  if (p_cell != NULL) {
    hashtable_setCurrentVar(ght_algo, p_cell);
    return p_cell;
  }

  return nullptr;
}


//loadVar: Met en haut de la pile, la valeur  du paramètre ou Variable-Local
//  correspondant à p_cell.
void loadVar(algo_cell *a_cell, var_cell *p_cell, bool isParam) {
  printf("\tcp ax,bp\n");
  int step;
  if (isParam) {
    //printf("----------- NUM PARAM : %d\n", p_cell->num);
    step = 4 + (2 * a_cell->param) - (2 * p_cell->num);
  } else {
    //printf("----------- NUM VAR LOC : %d\n", p_cell->num);
    step = 4 + (2 * a_cell->param) + (2 * a_cell->var_loc) - (2 * p_cell->num);
  }
  printf("\tconst bx,%d\n", step);
  printf("\tsub ax,bx\n");
  printf("\tloadw bx,ax\n");
  printf("\tpush bx\n");
}

//storeParam: Met dans la zone reservé des variables local de la pile associé à p_cell,
//  la valeur en haut de la pile.
void storeVar(algo_cell *a_cell, var_cell *p_cell) {
  printf("\tcp ax,bp\n");
  int step = 4 + (2 * a_cell->param) + (2 * a_cell->var_loc) - (2 * p_cell->num);
  printf("\tconst bx,%d\n", step);
  printf("\tsub ax,bx\n");
  printf("\tpop bx\n");
  printf("\tstorew bx,ax\n");
}

void storeReturnVal(algo_cell *a_cell) {
  printf("\tcp ax,bp\n");
  int step = 4 + (2 * a_cell->param) + (2 * a_cell->var_loc);
  printf("\tconst bx,%d\n", step);
  printf("\tsub ax,bx\n");
  printf("\tpop bx\n");
  printf("\tstorew bx,ax\n");
}

void storeContext() {
  printf("\tpush ax\n");
  printf("\tpush bx\n");
  printf("\tpush cx\n");
  printf("\tpush dx\n");
}

void restoreContext() {
  printf("\tpop dx\n");
  printf("\tpop cx\n");
  printf("\tpop bx\n");
  printf("\tpop ax\n");
}

void assignType(type_s *p, type_s t) {
    if (*p == UNDEF) {
        var_cell *v_cell = hashtable_getCurrentVar(ght_algo);
        if (v_cell != nullptr) {
            v_cell->type = t;
            hashtable_setCurrentVar(ght_algo, nullptr);
        } else {
            call_cell *c_cell = hashtable_getCurrentCall(ght_algo);
            if (c_cell != nullptr) {
              if (c_cell->expected_type != t && c_cell->expected_type == UNDEF) {
                c_cell->expected_type = t;
              }
            }
        }
        *p = t;
    }
}

var_cell *var_cell_search(ctrl_var *ctrl, char *ref) {
  var_cell *v_cell = ctrl->head;
  while (v_cell != nullptr) {
    if (strcmp(v_cell->name, ref) == 0) {
      return v_cell;
    }
    v_cell = v_cell->next;
  }
  return nullptr;
}

void var_cell_add(ctrl_var *ctrl, char *ref, type_s type) {
  var_cell *v_cell = malloc(sizeof(*v_cell));
  if (v_cell == nullptr) {
    printf("not enough memory\n");
    exit(EXIT_FAILURE);
  }

  v_cell->type = type;
  v_cell->num = ctrl->entry + 1;
  v_cell->name = strdup(ref);
  v_cell->next = nullptr;

  if (ctrl->head == nullptr) {
    ctrl->head = v_cell;
  } else {
    ctrl->tail->next = v_cell;
  }

  ctrl->tail = v_cell;
  ctrl->entry += 1;
  hashtable_setCurrentVar(ght_algo, v_cell);
}

var_cell *get_param_cell_Xnum(algo_cell *cell, size_t num) {
  var_cell *p = cell->ctrl_param->head;
  size_t count = 1;
  while (p != nullptr && count < num) {
    count++;
    p = p->next;
  }
  return p;
}

size_t str_hashfun(const char *s) {
  size_t h = 0;
  for (const unsigned char *p = (const unsigned char *) s; *p != '\0'; ++p) {
    h = 37 * h + *p;
  }
  return h;
}

void call_restore_pile(char *ref) {
  algo_cell *a_cell = hashtable_search(ght_algo, ref);
  int v = a_cell->param + a_cell->var_loc;
  for (int k = 0; k < v; k++) {
    printf("\tpop ax\n");
  }
}

int resolveCall(void *ref) {
    call_cell *c_cell = ref;

    if (c_cell == nullptr) {
        return 0;
    }

    if (c_cell->algo == nullptr) {
        printf("Call: algo inconnu\n");
        exit(EXIT_FAILURE);
    }

    if (c_cell->cv_cell == nullptr) {
        printf("Call: structure paramètres non initialisée\n");
        exit(EXIT_FAILURE);
    }

    if (c_cell->cv_cell->entry != a_cell->ctrl_param->entry) {
        printf("Call: nombre de paramètres incorrect pour '%s' (attendu %d, reçu %d)\n",
               a_cell->name,
               a_cell->ctrl_param->entry,
               c_cell->cv_cell->entry);
        exit(EXIT_FAILURE);
    }

    var_cell *expected = a_cell->ctrl_param->head;
    var_cell *given    = c_cell->cv_cell->head;

    while (expected != nullptr && given != nullptr) {

        if (expected->num != given->num) {
            printf("Call: numéro de paramètre incorrect pour '%s'\n",
                   a_cell->name);
            exit(EXIT_FAILURE);
        }

        if (expected->type != given->type) {
          if (expected->type == UNDEF) {
            expected->type = given->type;
          } else {
            printf("Call: type incorrect pour le paramètre %d de '%s'\n",
                   expected->num,
                   a_cell->name);
            exit(EXIT_FAILURE);
          }
        }

        expected = expected->next;
        given    = given->next;
    }

    return 0;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("fichier manquant\n");
    exit(EXIT_FAILURE);
  }

  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    printf("not enought memory\n");
    exit(EXIT_FAILURE);
  }

  ght_algo = hashtable_empty((int (*)(const void *, const void *))strcoll,
      (size_t (*)(const void *))str_hashfun, 1);

  ghl_call = holdall_empty();

  printf("\tconst ax,main\n");
  printf("\tjmp ax\n");
  
  isFirstParse = true;
  yyin = file;
  yyparse();

  isFirstParse = false;
  rewind(file);
  yyparse();

  //GESTION DES CALL
  holdall_apply(ghl_call, resolveCall);
  //VERIFICATION TYPE RETOUR FONCTION

  return EXIT_SUCCESS;
}