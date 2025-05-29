#include "../include/ast.h"
#include <stdio.h>
#include <stdlib.h>

struct program *ast_prog(struct decl_funcvar *funcvar,
                         struct decl_prog *decl_prog) {

  struct program *p = (struct program *)malloc(sizeof(*p));

  if (p) {
    p->prog = decl_prog;
    p->funcvar = funcvar;
  }

  return p;
}

struct decl_funcvar *ast_decl_funcvar(type_t type, char *id,
                                      struct decl_var *decl_var,
                                      struct decl_func *decl_func,
                                      struct decl_funcvar *next) {

  struct decl_funcvar *df = (struct decl_funcvar *)malloc(sizeof(*df));

  if (df) {
    df->type = type;
    df->id = id;
    df->decl_var = decl_var;
    df->decl_func = decl_func;
    df->next = next;
  }

  return df;
}
