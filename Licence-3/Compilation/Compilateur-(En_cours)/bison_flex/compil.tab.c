/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "compil.y"

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

#line 148 "compil.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "compil.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUM = 3,                        /* NUM  */
  YYSYMBOL_LE = 4,                         /* LE  */
  YYSYMBOL_GE = 5,                         /* GE  */
  YYSYMBOL_DIFF = 6,                       /* DIFF  */
  YYSYMBOL_TRUE = 7,                       /* TRUE  */
  YYSYMBOL_FALSE = 8,                      /* FALSE  */
  YYSYMBOL_AND = 9,                        /* AND  */
  YYSYMBOL_OR = 10,                        /* OR  */
  YYSYMBOL_XOR = 11,                       /* XOR  */
  YYSYMBOL_NOT = 12,                       /* NOT  */
  YYSYMBOL_B_ALGO = 13,                    /* B_ALGO  */
  YYSYMBOL_E_ALGO = 14,                    /* E_ALGO  */
  YYSYMBOL_ID = 15,                        /* ID  */
  YYSYMBOL_RET = 16,                       /* RET  */
  YYSYMBOL_SET = 17,                       /* SET  */
  YYSYMBOL_FOR = 18,                       /* FOR  */
  YYSYMBOL_WHILE = 19,                     /* WHILE  */
  YYSYMBOL_CALL = 20,                      /* CALL  */
  YYSYMBOL_IF = 21,                        /* IF  */
  YYSYMBOL_ELSE = 22,                      /* ELSE  */
  YYSYMBOL_FI = 23,                        /* FI  */
  YYSYMBOL_OD = 24,                        /* OD  */
  YYSYMBOL_25_ = 25,                       /* '<'  */
  YYSYMBOL_26_ = 26,                       /* '>'  */
  YYSYMBOL_27_ = 27,                       /* '='  */
  YYSYMBOL_28_ = 28,                       /* '+'  */
  YYSYMBOL_29_ = 29,                       /* '-'  */
  YYSYMBOL_30_ = 30,                       /* '*'  */
  YYSYMBOL_31_ = 31,                       /* '/'  */
  YYSYMBOL_32_ = 32,                       /* '('  */
  YYSYMBOL_33_ = 33,                       /* ')'  */
  YYSYMBOL_34_ = 34,                       /* '{'  */
  YYSYMBOL_35_ = 35,                       /* '}'  */
  YYSYMBOL_36_ = 36,                       /* ','  */
  YYSYMBOL_YYACCEPT = 37,                  /* $accept  */
  YYSYMBOL_PROG = 38,                      /* PROG  */
  YYSYMBOL_ALGO_LIST = 39,                 /* ALGO_LIST  */
  YYSYMBOL_E_CALL = 40,                    /* E_CALL  */
  YYSYMBOL_41_1 = 41,                      /* $@1  */
  YYSYMBOL_EXPR_CALL = 42,                 /* EXPR_CALL  */
  YYSYMBOL_CALL_INIT = 43,                 /* CALL_INIT  */
  YYSYMBOL_LVALUE = 44,                    /* LVALUE  */
  YYSYMBOL_VALUE_LIST = 45,                /* VALUE_LIST  */
  YYSYMBOL_VALUE = 46,                     /* VALUE  */
  YYSYMBOL_ALGO_NAME_INIT = 47,            /* ALGO_NAME_INIT  */
  YYSYMBOL_ALGO = 48,                      /* ALGO  */
  YYSYMBOL_PARAM_BLOCK = 49,               /* PARAM_BLOCK  */
  YYSYMBOL_PARAM_LIST = 50,                /* PARAM_LIST  */
  YYSYMBOL_LPARAM = 51,                    /* LPARAM  */
  YYSYMBOL_PARAM = 52,                     /* PARAM  */
  YYSYMBOL_LEXPR = 53,                     /* LEXPR  */
  YYSYMBOL_LINSTR_BLOCK = 54,              /* LINSTR_BLOCK  */
  YYSYMBOL_LINSTR = 55,                    /* LINSTR  */
  YYSYMBOL_TAG_INIT = 56,                  /* TAG_INIT  */
  YYSYMBOL_INSTR = 57,                     /* INSTR  */
  YYSYMBOL_58_2 = 58,                      /* $@2  */
  YYSYMBOL_59_3 = 59,                      /* $@3  */
  YYSYMBOL_60_4 = 60,                      /* $@4  */
  YYSYMBOL_61_5 = 61,                      /* $@5  */
  YYSYMBOL_62_6 = 62,                      /* $@6  */
  YYSYMBOL_63_7 = 63,                      /* $@7  */
  YYSYMBOL_IF_STMT = 64,                   /* IF_STMT  */
  YYSYMBOL_EXPR = 65,                      /* EXPR  */
  YYSYMBOL_66_8 = 66,                      /* $@8  */
  YYSYMBOL_67_9 = 67,                      /* $@9  */
  YYSYMBOL_68_10 = 68,                     /* $@10  */
  YYSYMBOL_69_11 = 69,                     /* $@11  */
  YYSYMBOL_70_12 = 70,                     /* $@12  */
  YYSYMBOL_71_13 = 71,                     /* $@13  */
  YYSYMBOL_72_14 = 72,                     /* $@14  */
  YYSYMBOL_73_15 = 73,                     /* $@15  */
  YYSYMBOL_74_16 = 74,                     /* $@16  */
  YYSYMBOL_75_17 = 75,                     /* $@17  */
  YYSYMBOL_76_18 = 76,                     /* $@18  */
  YYSYMBOL_77_19 = 77,                     /* $@19  */
  YYSYMBOL_78_20 = 78                      /* $@20  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   251

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  75
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  143

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   279


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      32,    33,    30,    28,    36,    29,     2,    31,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      25,    27,    26,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    34,     2,    35,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   135,   135,   136,   140,   141,   146,   146,   185,   212,
     252,   253,   257,   258,   262,   303,   350,   361,   365,   366,
     370,   371,   376,   390,   391,   395,   396,   400,   401,   405,
     415,   434,   463,   476,   462,   535,   543,   534,   574,   591,
     573,   611,   612,   617,   620,   619,   635,   634,   649,   648,
     663,   662,   676,   678,   686,   705,   712,   720,   719,   733,
     732,   746,   745,   759,   758,   772,   771,   785,   784,   798,
     797,   817,   816,   836,   835,   854
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUM", "LE", "GE",
  "DIFF", "TRUE", "FALSE", "AND", "OR", "XOR", "NOT", "B_ALGO", "E_ALGO",
  "ID", "RET", "SET", "FOR", "WHILE", "CALL", "IF", "ELSE", "FI", "OD",
  "'<'", "'>'", "'='", "'+'", "'-'", "'*'", "'/'", "'('", "')'", "'{'",
  "'}'", "','", "$accept", "PROG", "ALGO_LIST", "E_CALL", "$@1",
  "EXPR_CALL", "CALL_INIT", "LVALUE", "VALUE_LIST", "VALUE",
  "ALGO_NAME_INIT", "ALGO", "PARAM_BLOCK", "PARAM_LIST", "LPARAM", "PARAM",
  "LEXPR", "LINSTR_BLOCK", "LINSTR", "TAG_INIT", "INSTR", "$@2", "$@3",
  "$@4", "$@5", "$@6", "$@7", "IF_STMT", "EXPR", "$@8", "$@9", "$@10",
  "$@11", "$@12", "$@13", "$@14", "$@15", "$@16", "$@17", "$@18", "$@19",
  "$@20", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-80)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -10,   -29,    12,   -10,   -80,     0,   -12,   -80,   -80,     9,
     -80,     2,     5,     7,     8,    11,   -80,   -80,    15,    22,
     -80,    29,    31,    34,   -80,   -80,    46,     7,   -80,    51,
     144,   -80,     5,   144,    56,    57,   -80,    39,   -80,   -80,
      53,   -80,   -80,   -80,   144,   -80,   144,   -80,    63,    55,
      66,   -80,   144,   168,   -80,    -1,    69,    71,    74,   144,
     -80,   211,   157,   144,   -80,   144,   168,   -80,   -80,   -80,
     -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,
     -80,    76,   -80,   144,     1,   -80,    80,   -80,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   -80,    32,   -80,   -80,   113,   113,   113,   211,
     196,   204,   113,   113,   113,   -13,   -13,   -80,   -80,   104,
      84,   -80,     7,   -80,   144,     7,     7,   114,    38,    26,
      86,   -80,     7,   -80,   -80,   144,    77,   125,   -80,   -80,
       7,   227,   -80
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     0,     6,     5,     0,     0,     1,     2,     0,
       4,     0,    19,    26,     0,     0,    15,    22,     0,    18,
      21,     0,     0,     0,    29,    29,     0,    25,    28,     0,
      11,    17,     0,     0,     0,     0,    35,     0,    16,    27,
       0,    53,    55,    56,     0,    54,     0,    43,     0,     0,
      10,    13,    14,    24,    20,     0,     0,     0,     0,     0,
       9,    75,     0,    11,     7,     0,    23,    61,    63,    67,
      69,    71,    73,    57,    59,    65,    44,    46,    48,    50,
      30,     0,    32,     0,     0,    52,     0,    12,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,    38,     8,    62,    64,    68,    70,
      72,    74,    58,    60,    66,    45,    47,    49,    51,     0,
       0,    36,     0,    31,     0,     0,    39,     0,     0,     0,
       0,    37,     0,    41,    40,     0,     0,     0,    42,    33,
       0,     0,    34
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -80,   -80,   -80,   -80,   -80,   -80,    82,    62,   -80,    65,
     -80,   124,   -80,   -80,   -80,    99,   -32,   -80,   -79,   -15,
     -27,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -14,   -80,
     -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,
     -80,   -80
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     8,     9,    47,    48,    49,    50,    51,
       6,     4,    13,    18,    19,    20,    52,    26,    27,    36,
      28,   102,   140,    58,   125,   122,   129,   134,    53,    97,
      98,    99,   100,    94,    95,    88,    89,    96,    90,    91,
      92,    93
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      39,    55,    41,     1,    41,     5,    42,    43,    42,    43,
      37,    44,     7,    44,    45,    11,    45,    78,    79,    14,
      17,    14,    12,    21,    22,    23,    24,    84,    25,    14,
      61,    46,    62,    46,    80,    41,   104,    16,    66,    42,
      43,    66,    29,   126,    44,    30,   128,    45,   132,   133,
      31,   103,    14,   136,    21,    22,    23,    24,    32,    25,
      38,   141,   131,    33,    46,    34,    40,   121,    35,   119,
      66,    56,    57,    59,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   120,    60,    66,
      64,    15,   127,    21,    22,    23,    24,    63,    25,    39,
     138,    39,    65,   137,    81,    66,    82,    41,    83,    39,
     101,    42,    43,    66,    39,   105,    44,    41,   124,    45,
     135,    42,    43,    66,    14,    86,    44,    10,    41,    45,
      87,    54,    42,    43,    14,     0,    46,    44,     0,   123,
      45,    76,    77,    78,    79,    14,    46,    41,     0,   130,
       0,    42,    43,     0,     0,     0,    44,    46,     0,    45,
     139,    67,    68,    69,    14,     0,    70,    71,    72,     0,
       0,     0,    67,    68,    69,     0,    46,    70,    71,    72,
       0,     0,    73,    74,    75,    76,    77,    78,    79,     0,
      85,     0,     0,    73,    74,    75,    76,    77,    78,    79,
      67,    68,    69,     0,     0,    70,     0,    72,    67,    68,
      69,     0,     0,    70,     0,    67,    68,    69,     0,     0,
       0,    73,    74,    75,    76,    77,    78,    79,     0,    73,
      74,    75,    76,    77,    78,    79,    73,    74,    75,    76,
      77,    78,    79,    21,    22,    23,    24,     0,    25,     0,
       0,   142
};

static const yytype_int16 yycheck[] =
{
      27,    33,     3,    13,     3,    34,     7,     8,     7,     8,
      25,    12,     0,    12,    15,    15,    15,    30,    31,    20,
      15,    20,    34,    16,    17,    18,    19,    59,    21,    20,
      44,    32,    46,    32,    35,     3,    35,    35,    52,     7,
       8,    55,    34,   122,    12,    34,   125,    15,    22,    23,
      35,    83,    20,   132,    16,    17,    18,    19,    36,    21,
      14,   140,    24,    34,    32,    34,    15,    35,    34,   101,
      84,    15,    15,    34,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   102,    35,   103,
      35,     9,   124,    16,    17,    18,    19,    34,    21,   126,
      23,   128,    36,   135,    35,   119,    35,     3,    34,   136,
      34,     7,     8,   127,   141,    35,    12,     3,    34,    15,
      34,     7,     8,   137,    20,    63,    12,     3,     3,    15,
      65,    32,     7,     8,    20,    -1,    32,    12,    -1,    35,
      15,    28,    29,    30,    31,    20,    32,     3,    -1,    35,
      -1,     7,     8,    -1,    -1,    -1,    12,    32,    -1,    15,
      35,     4,     5,     6,    20,    -1,     9,    10,    11,    -1,
      -1,    -1,     4,     5,     6,    -1,    32,     9,    10,    11,
      -1,    -1,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    -1,    -1,    25,    26,    27,    28,    29,    30,    31,
       4,     5,     6,    -1,    -1,     9,    -1,    11,     4,     5,
       6,    -1,    -1,     9,    -1,     4,     5,     6,    -1,    -1,
      -1,    25,    26,    27,    28,    29,    30,    31,    -1,    25,
      26,    27,    28,    29,    30,    31,    25,    26,    27,    28,
      29,    30,    31,    16,    17,    18,    19,    -1,    21,    -1,
      -1,    24
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    13,    38,    39,    48,    34,    47,     0,    40,    41,
      48,    15,    34,    49,    20,    43,    35,    15,    50,    51,
      52,    16,    17,    18,    19,    21,    54,    55,    57,    34,
      34,    35,    36,    34,    34,    34,    56,    56,    14,    57,
      15,     3,     7,     8,    12,    15,    32,    42,    43,    44,
      45,    46,    53,    65,    52,    53,    15,    15,    60,    34,
      35,    65,    65,    34,    35,    36,    65,     4,     5,     6,
       9,    10,    11,    25,    26,    27,    28,    29,    30,    31,
      35,    35,    35,    34,    53,    33,    44,    46,    72,    73,
      75,    76,    77,    78,    70,    71,    74,    66,    67,    68,
      69,    34,    58,    53,    35,    35,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    53,
      56,    35,    62,    35,    34,    61,    55,    53,    55,    63,
      35,    24,    22,    23,    64,    34,    55,    53,    23,    35,
      59,    55,    24
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    37,    38,    38,    39,    39,    41,    40,    42,    43,
      44,    44,    45,    45,    46,    47,    48,    49,    50,    50,
      51,    51,    52,    53,    53,    54,    54,    55,    55,    56,
      57,    57,    58,    59,    57,    60,    61,    57,    62,    63,
      57,    64,    64,    65,    66,    65,    67,    65,    68,    65,
      69,    65,    65,    65,    65,    65,    65,    70,    65,    71,
      65,    72,    65,    73,    65,    74,    65,    75,    65,    76,
      65,    77,    65,    78,    65,    65
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     2,     1,     0,     5,     4,     4,
       1,     0,     3,     1,     1,     3,     5,     3,     1,     0,
       3,     1,     1,     2,     1,     1,     0,     2,     1,     0,
       4,     7,     0,     0,    15,     0,     0,     9,     0,     0,
       9,     1,     3,     1,     0,     4,     0,     4,     0,     4,
       0,     4,     3,     1,     1,     1,     1,     0,     4,     0,
       4,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       4,     0,     4,     0,     4,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 6: /* $@1: %empty  */
#line 146 "compil.y"
    {
      if (!isFirstParse) {
        printf(":main\n");
        printf("\tconst bp,stack\n");
        printf("\tconst sp,stack\n");
        printf("\tconst ax,2\n");
        printf("\tsub sp,ax\n");
      }
    }
#line 1339 "compil.tab.c"
    break;

  case 7: /* E_CALL: $@1 CALL_INIT '{' LVALUE '}'  */
#line 156 "compil.y"
    {
      if (!isFirstParse) {
        call_cell *c_cell = (yyvsp[-3].call);
  
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
#line 1368 "compil.tab.c"
    break;

  case 8: /* EXPR_CALL: CALL_INIT '{' LVALUE '}'  */
#line 186 "compil.y"
  { if (isFirstParse) {
        (yyval.t) = UNDEF;
      } else {
        call_cell *c_cell = (yyvsp[-3].call);

        hashtable_setCurrentCall(ght_algo, c_cell);

        algo_cell *a_cell = c_cell->algo;
        printf("\tconst ax,%s\n", c_cell->algo_name);
        printf("\tcall ax\n");

        (yyval.t) = a_cell == nullptr ? UNDEF : a_cell->type;

        int entry = a_cell->ctrl_param->entry +
                    a_cell->ctrl_var_loc->entry;

        for (int k = 0; k < entry; k++) {
          printf("\tpop ax\n");
        }
        call_cell *finished = (yyvsp[-3].call);
        hashtable_setCurrentCall(ght_algo, finished->prev);
      }
    }
#line 1396 "compil.tab.c"
    break;

  case 9: /* CALL_INIT: CALL '{' ID '}'  */
#line 213 "compil.y"
    {
      if (isFirstParse) {
        (yyval.call) = nullptr;
      } else {
        call_cell *c_cell = malloc(sizeof(*c_cell));
        if (!c_cell) {
          printf("not enough memory\n");
            exit(EXIT_FAILURE);
        }
        c_cell->prev = hashtable_getCurrentCall(ght_algo);
        hashtable_setCurrentCall(ght_algo, c_cell);
        lastCall = c_cell;
        
        algo_cell *target = hashtable_search(ght_algo, (yyvsp[-1].string));
        c_cell->algo = target;
        c_cell->algo_name = (yyvsp[-1].string);
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
        algo_cell *a_cell = hashtable_search(ght_algo, (yyvsp[-1].string));
        for (int k = 0; k < a_cell->ctrl_var_loc->entry + 1; k++) {
          printf("\tpush ax\n");
        }

        (yyval.call) = c_cell;
      }
    }
#line 1437 "compil.tab.c"
    break;

  case 14: /* VALUE: LEXPR  */
#line 263 "compil.y"
    {
      if (!isFirstParse) {
        var_cell *v_cell = malloc(sizeof(*v_cell));
        if (v_cell == nullptr) {
          printf("not enough memory\n");
          exit(EXIT_FAILURE);
        }
        call_cell *c_cell = hashtable_getCurrentCall(ght_algo);
        v_cell->type = (yyvsp[0].t);
        v_cell->num = c_cell->cv_cell->entry + 1;
        v_cell->next = nullptr; 
        if (c_cell->cv_cell->head == nullptr) {
          c_cell->cv_cell->head = v_cell;
        } else {
          c_cell->cv_cell->tail->next = v_cell;
        }
        c_cell->cv_cell->tail = v_cell;
        c_cell->cv_cell->entry += 1;

        algo_cell *a_cell = hashtable_search(ght_algo, c_cell->algo_name);
        if (a_cell == nullptr) {
          printf("Algo inconnue\n");
          exit(EXIT_FAILURE);
        }
        v_cell = get_param_cell_Xnum(a_cell, c_cell->cv_cell->entry);
        if (v_cell == nullptr) {
          printf("To many parameters in call\n");
          exit(EXIT_FAILURE);
        }
        if (v_cell->type == UNDEF) {
          v_cell->type = (yyvsp[0].t);
        } else if (v_cell->type != (yyvsp[0].t)) {
          printf("Incompatible parameter type\n");
          exit(EXIT_FAILURE);
        }
      }
    }
#line 1479 "compil.tab.c"
    break;

  case 15: /* ALGO_NAME_INIT: '{' ID '}'  */
#line 304 "compil.y"
    {
      algo_name = (yyvsp[-1].string);

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
#line 1526 "compil.tab.c"
    break;

  case 16: /* ALGO: B_ALGO ALGO_NAME_INIT PARAM_BLOCK LINSTR_BLOCK E_ALGO  */
#line 351 "compil.y"
    {
        if (isFirstParse) {
            algo_cell *a_cell = hashtable_getCurrentAlgo(ght_algo);
            a_cell->var_loc = a_cell->ctrl_var_loc->entry;
            hashtable_setCurrentCall(ght_algo, nullptr);
        }
    }
#line 1538 "compil.tab.c"
    break;

  case 22: /* PARAM: ID  */
#line 376 "compil.y"
     {
        if (isFirstParse) {
          algo_cell *a_cell = hashtable_getCurrentAlgo(ght_algo);
          var_cell *c = var_cell_search(a_cell->ctrl_param, (yyvsp[0].string));
          if (c != NULL) {
            printf("La variable '%s' existe déjà dans cet algo\n", (yyvsp[0].string));
            exit(EXIT_FAILURE);
          }
          var_cell_add(a_cell->ctrl_param, (yyvsp[0].string), UNDEF);
        }
      }
#line 1554 "compil.tab.c"
    break;

  case 29: /* TAG_INIT: %empty  */
#line 406 "compil.y"
    {
      if (!isFirstParse)
          (yyval.integer) = etiquette();
      else
          (yyval.integer) = 0;
    }
#line 1565 "compil.tab.c"
    break;

  case 30: /* INSTR: RET '{' LEXPR '}'  */
#line 416 "compil.y"
      {
        algo_cell *a_cell = hashtable_getCurrentAlgo(ght_algo);
        type_s t = a_cell->type;
        if (t == UNDEF || t == (yyvsp[-1].t)) {
          a_cell->type = (yyvsp[-1].t);
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
#line 1587 "compil.tab.c"
    break;

  case 31: /* INSTR: SET '{' ID '}' '{' LEXPR '}'  */
#line 435 "compil.y"
      {
        algo_cell *a_cell = hashtable_getCurrentAlgo(ght_algo);
        var_cell *p_cell = var_cell_search(a_cell->ctrl_param, (yyvsp[-4].string));

        if (p_cell == nullptr) {
          p_cell = var_cell_search(a_cell->ctrl_var_loc, (yyvsp[-4].string));
          if (p_cell == nullptr) {
            //initialisation
            if (isFirstParse) {
              var_cell_add(a_cell->ctrl_var_loc, (yyvsp[-4].string), (yyvsp[-1].t));
            }
          } else {
            //update value
            if (p_cell->type != (yyvsp[-1].t)) {
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
#line 1618 "compil.tab.c"
    break;

  case 32: /* $@2: %empty  */
#line 463 "compil.y"
      {
        if (isFirstParse) {
          var_cell *p_cell = getVar_In_SymbolTable((yyvsp[-1].string));
          if (p_cell != nullptr) {
            printf("Erreur: variable déjà utilisée\n");
            exit(EXIT_FAILURE);
          }
          algo_cell *a_cell = hashtable_getCurrentAlgo(ght_algo);
          var_cell_add(a_cell->ctrl_var_loc, (yyvsp[-1].string), INT_T);
        }
      }
#line 1634 "compil.tab.c"
    break;

  case 33: /* $@3: %empty  */
#line 476 "compil.y"
      {
        if (!isFirstParse) {

          if ((yyvsp[-4].t) == UNDEF) {
            assignType(&(yyvsp[-4].t), INT_T);
          }
          if ((yyvsp[-1].t) == UNDEF) {
            assignType(&(yyvsp[-1].t), INT_T);
          }
          if ((yyvsp[-4].t) != INT_T || (yyvsp[-1].t) != INT_T) {
            printf("Erreur: erreur de typage, expecting INT_T\n");
            exit(EXIT_FAILURE);
          }
          
          int e = (yyvsp[-6].integer);
          //début boucle
          printf(":dofori_%d\n", e);
          //store varLoc
          algo_cell *a_cell = hashtable_getCurrentAlgo(ght_algo);
          var_cell *p_cell = var_cell_search(a_cell->ctrl_var_loc ,(yyvsp[-9].string));
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
#line 1680 "compil.tab.c"
    break;

  case 34: /* INSTR: FOR '{' ID '}' $@2 TAG_INIT '{' LEXPR '}' '{' LEXPR '}' $@3 LINSTR OD  */
#line 518 "compil.y"
      {
        if (!isFirstParse) {
          int e = (yyvsp[-9].integer);
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
#line 1700 "compil.tab.c"
    break;

  case 35: /* $@4: %empty  */
#line 535 "compil.y"
    {
      if (!isFirstParse) {
        int e = (yyvsp[0].integer);
        // début boucle
        printf(":while_%d\n", e);
      }
    }
#line 1712 "compil.tab.c"
    break;

  case 36: /* $@5: %empty  */
#line 543 "compil.y"
    {
      if (isFirstParse) {
          if ((yyvsp[-4].integer) != BOOL_T) {
            printf("Erreur: erreur de typage while, expecting BOOL_T\n");
            exit(EXIT_FAILURE);
          }
        } else {
          int e = (yyvsp[-4].integer);
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
#line 1735 "compil.tab.c"
    break;

  case 37: /* INSTR: WHILE TAG_INIT $@4 '{' LEXPR '}' $@5 LINSTR OD  */
#line 562 "compil.y"
    {
      if (!isFirstParse) {
        int e = (yyvsp[-7].integer);
        //goto début
        printf("\tconst cx,while_%d\n", e);
        printf("\tjmp cx\n");
        printf(":end_while_%d\n", e);
      }
    }
#line 1749 "compil.tab.c"
    break;

  case 38: /* $@6: %empty  */
#line 574 "compil.y"
    {
      if (isFirstParse) {
        if ((yyvsp[-3].integer) != BOOL_T) {
          printf("Erreur: erreur de typage IF, expecting BOOL_T\n");
          exit(EXIT_FAILURE);
        }
      } else {
        int e = (yyvsp[-3].integer);
        // On saute après les instructions dans le SI si on a faux
        printf("\tconst cx,end_if_%d\n", e);
        printf("\tpop ax\n");
        printf("\tconst bx,0\n");
        printf("\tcmp ax,bx\n");
        printf("\tjmpc cx\n");
      }
    }
#line 1770 "compil.tab.c"
    break;

  case 39: /* $@7: %empty  */
#line 591 "compil.y"
    {
      if (!isFirstParse) {
        int e = (yyvsp[-5].integer);
        // Si on vient des instructions du SI on saute après l'éventuel else
        printf("\tconst cx,end_else_%d\n", e);
        printf("\tjmp cx\n");
        // Après cette étiquette soit on a rien soit on a les instructions dans le else
        printf(":end_if_%d\n", e);
      }
    }
#line 1785 "compil.tab.c"
    break;

  case 40: /* INSTR: IF TAG_INIT '{' LEXPR '}' $@6 LINSTR $@7 IF_STMT  */
#line 602 "compil.y"
    {
      if (!isFirstParse) {
        int e = (yyvsp[-7].integer);
        printf(":end_else_%d\n", e);
      }
    }
#line 1796 "compil.tab.c"
    break;

  case 44: /* $@8: %empty  */
#line 620 "compil.y"
      {
        assignType(&(yyvsp[-1].t), INT_T);
      }
#line 1804 "compil.tab.c"
    break;

  case 45: /* EXPR: EXPR '+' $@8 EXPR  */
#line 624 "compil.y"
      {
        assignType(&(yyvsp[0].t), INT_T);
        if (bin_opr((yyvsp[-3].t), (yyvsp[0].t), "add", isFirstParse) == INT_T) {
            (yyval.t)=INT_T;
          } else {
            printf("Erreur de typage\n");
            exit(EXIT_FAILURE);
        }
      }
#line 1818 "compil.tab.c"
    break;

  case 46: /* $@9: %empty  */
#line 635 "compil.y"
      {
        assignType(&(yyvsp[-1].t), INT_T);
      }
#line 1826 "compil.tab.c"
    break;

  case 47: /* EXPR: EXPR '-' $@9 EXPR  */
#line 638 "compil.y"
         {
          assignType(&(yyvsp[0].t), INT_T);
          if (bin_opr((yyvsp[-3].t), (yyvsp[0].t), "sub", isFirstParse) == INT_T) {
            (yyval.t)=INT_T;
          } else {
            printf("Erreur de typage\n");
            exit(EXIT_FAILURE);
          }
        }
#line 1840 "compil.tab.c"
    break;

  case 48: /* $@10: %empty  */
#line 649 "compil.y"
      {
        assignType(&(yyvsp[-1].t), INT_T);
      }
#line 1848 "compil.tab.c"
    break;

  case 49: /* EXPR: EXPR '*' $@10 EXPR  */
#line 652 "compil.y"
         {
            assignType(&(yyvsp[0].t), INT_T);
            if (bin_opr((yyvsp[-3].t), (yyvsp[0].t), "mul", isFirstParse) == INT_T) {
              (yyval.t)=INT_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
#line 1862 "compil.tab.c"
    break;

  case 50: /* $@11: %empty  */
#line 663 "compil.y"
      {
        assignType(&(yyvsp[-1].t), INT_T);
      }
#line 1870 "compil.tab.c"
    break;

  case 51: /* EXPR: EXPR '/' $@11 EXPR  */
#line 666 "compil.y"
          {
            assignType(&(yyvsp[0].t), INT_T);
            if (div_opr((yyvsp[-3].t), (yyvsp[0].t), isFirstParse) == INT_T) {
              (yyval.t)=INT_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
#line 1884 "compil.tab.c"
    break;

  case 52: /* EXPR: '(' EXPR ')'  */
#line 676 "compil.y"
                 {(yyval.t)=(yyvsp[-1].t);}
#line 1890 "compil.tab.c"
    break;

  case 53: /* EXPR: NUM  */
#line 678 "compil.y"
          {
            if (!isFirstParse) {
              printf("\tconst ax,%d\n", (yyvsp[0].integer));
              printf("\tpush ax\n");
            }
            (yyval.t)=INT_T;
          }
#line 1902 "compil.tab.c"
    break;

  case 54: /* EXPR: ID  */
#line 686 "compil.y"
          {
            var_cell *p_cell;
            p_cell = getVar_In_SymbolTable((yyvsp[0].string));
            if (p_cell == nullptr) {
                printf("Erreur: variable %s inconue au bataillon\n", (yyvsp[0].string));
                exit(EXIT_FAILURE);
            }
            (yyval.t)=p_cell->type;
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
#line 1925 "compil.tab.c"
    break;

  case 55: /* EXPR: TRUE  */
#line 705 "compil.y"
          {
            if (!isFirstParse) {
              printf("\tconst ax,1\n");
              printf("\tpush ax\n");
            }
            (yyval.t)=BOOL_T;
          }
#line 1937 "compil.tab.c"
    break;

  case 56: /* EXPR: FALSE  */
#line 712 "compil.y"
          {
            if (!isFirstParse) {
              printf("\tconst ax,0\n");
              printf("\tpush ax\n");
            }
            (yyval.t)=BOOL_T;
          }
#line 1949 "compil.tab.c"
    break;

  case 57: /* $@12: %empty  */
#line 720 "compil.y"
      {
        assignType(&(yyvsp[-1].t), INT_T);
      }
#line 1957 "compil.tab.c"
    break;

  case 58: /* EXPR: EXPR '<' $@12 EXPR  */
#line 723 "compil.y"
          {
            assignType(&(yyvsp[0].t), INT_T);
            if(inf_bool((yyvsp[-3].t), (yyvsp[0].t), isFirstParse) == BOOL_T) {
              (yyval.t)=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
#line 1971 "compil.tab.c"
    break;

  case 59: /* $@13: %empty  */
#line 733 "compil.y"
      {
        assignType(&(yyvsp[-1].t), INT_T);
      }
#line 1979 "compil.tab.c"
    break;

  case 60: /* EXPR: EXPR '>' $@13 EXPR  */
#line 736 "compil.y"
          {
            assignType(&(yyvsp[0].t), INT_T);
            if(sup_bool((yyvsp[-3].t), (yyvsp[0].t), isFirstParse) == BOOL_T) {
              (yyval.t)=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
#line 1993 "compil.tab.c"
    break;

  case 61: /* $@14: %empty  */
#line 746 "compil.y"
      {
        assignType(&(yyvsp[-1].t), INT_T);
      }
#line 2001 "compil.tab.c"
    break;

  case 62: /* EXPR: EXPR LE $@14 EXPR  */
#line 749 "compil.y"
         {
            assignType(&(yyvsp[0].t), INT_T);
            if(inf_eq_bool((yyvsp[-3].t), (yyvsp[0].t), isFirstParse) == BOOL_T) {
              (yyval.t)=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
#line 2015 "compil.tab.c"
    break;

  case 63: /* $@15: %empty  */
#line 759 "compil.y"
      {
        assignType(&(yyvsp[-1].t), INT_T);
      }
#line 2023 "compil.tab.c"
    break;

  case 64: /* EXPR: EXPR GE $@15 EXPR  */
#line 762 "compil.y"
         {
            assignType(&(yyvsp[0].t), INT_T);
            if(sup_eq_bool((yyvsp[-3].t), (yyvsp[0].t), isFirstParse) == BOOL_T) {
              (yyval.t)=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
#line 2037 "compil.tab.c"
    break;

  case 65: /* $@16: %empty  */
#line 772 "compil.y"
      {
        assignType(&(yyvsp[-1].t), INT_T);
      }
#line 2045 "compil.tab.c"
    break;

  case 66: /* EXPR: EXPR '=' $@16 EXPR  */
#line 775 "compil.y"
          {
            assignType(&(yyvsp[0].t), INT_T);
            if(eq_bool((yyvsp[-3].t), (yyvsp[0].t), isFirstParse) == BOOL_T) {
              (yyval.t)=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
#line 2059 "compil.tab.c"
    break;

  case 67: /* $@17: %empty  */
#line 785 "compil.y"
      {
        assignType(&(yyvsp[-1].t), INT_T);
      }
#line 2067 "compil.tab.c"
    break;

  case 68: /* EXPR: EXPR DIFF $@17 EXPR  */
#line 788 "compil.y"
          {
            assignType(&(yyvsp[0].t), INT_T);
            if(diff_bool((yyvsp[-3].t), (yyvsp[0].t), isFirstParse) == BOOL_T) {
              (yyval.t)=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
#line 2081 "compil.tab.c"
    break;

  case 69: /* $@18: %empty  */
#line 798 "compil.y"
      {
        assignType(&(yyvsp[-1].t), BOOL_T);
      }
#line 2089 "compil.tab.c"
    break;

  case 70: /* EXPR: EXPR AND $@18 EXPR  */
#line 801 "compil.y"
          {
            assignType(&(yyvsp[0].t), BOOL_T);
            if((yyvsp[-3].t)==BOOL_T && (yyvsp[0].t)==BOOL_T) {
              if (!isFirstParse) {
                printf("\tpop ax\n");
                printf("\tpop bx\n");
                printf("\tand ax,bx\n");
                printf("\tpush ax\n");
              }
              (yyval.t)=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
#line 2109 "compil.tab.c"
    break;

  case 71: /* $@19: %empty  */
#line 817 "compil.y"
      {
        assignType(&(yyvsp[-1].t), BOOL_T);
      }
#line 2117 "compil.tab.c"
    break;

  case 72: /* EXPR: EXPR OR $@19 EXPR  */
#line 820 "compil.y"
          {
            assignType(&(yyvsp[0].t), BOOL_T);
            if((yyvsp[-3].t)==BOOL_T && (yyvsp[0].t)==BOOL_T) {
              if (!isFirstParse) {
                printf("\tpop ax\n");
                printf("\tpop bx\n");
                printf("\tor ax,bx\n");
                printf("\tpush ax\n");
              }
              (yyval.t)=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
#line 2137 "compil.tab.c"
    break;

  case 73: /* $@20: %empty  */
#line 836 "compil.y"
      {
        assignType(&(yyvsp[-1].t), BOOL_T);
      }
#line 2145 "compil.tab.c"
    break;

  case 74: /* EXPR: EXPR XOR $@20 EXPR  */
#line 839 "compil.y"
          {
            assignType(&(yyvsp[0].t), BOOL_T);
            if((yyvsp[-3].t)==BOOL_T && (yyvsp[0].t)==BOOL_T) {
              if (!isFirstParse) {
                printf("\tpop ax\n");
                printf("\tpop bx\n");
                printf("\txor ax,bx\n");
                printf("\tpush ax\n");
              }
              (yyval.t)=BOOL_T;
            } else {
              printf("Erreur de typage\n");
              exit(EXIT_FAILURE);
            }
          }
#line 2165 "compil.tab.c"
    break;

  case 75: /* EXPR: NOT EXPR  */
#line 854 "compil.y"
              {
                assignType(&(yyvsp[0].t), BOOL_T);
                if((yyvsp[0].t)==BOOL_T) {
                  if (!isFirstParse) {
                    printf("\tpop ax\n");
                    printf("\tnot ax\n");
                    printf("\tpush ax\n");
                  }
                  (yyval.t)=BOOL_T;
                } else {
                  printf("Erreur de typage\n");
                  exit(EXIT_FAILURE);
                }
              }
#line 2184 "compil.tab.c"
    break;


#line 2188 "compil.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 871 "compil.y"


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

    algo_cell *a_cell = hashtable_search(ght_algo, c_cell->algo->name);
    if (a_cell == nullptr) {
        printf("Call: L'algo '%s' est inconnue\n", c_cell->algo->name);
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
