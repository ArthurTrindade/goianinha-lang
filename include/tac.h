#ifndef TAC_H
#define TAC_H

#include <stdio.h>

/* O Conjunto de Instruções */
typedef enum {
  TAC_SYMBOL,     /* Um literal ou variável solta (a, 10) */
  TAC_ADD,        /* t0 = a + b */
  TAC_SUB,        /* t0 = a - b */
  TAC_MUL,        /* t0 = a * b */
  TAC_DIV,        /* t0 = a / b */
  TAC_MOVE,       /* a = b (Cópia) */
  TAC_IFZ,        /* if z == 0 goto L */
  TAC_LABEL,      /* Label: */
  TAC_JMP,        /* goto L */
  TAC_PRINT,      /* print a */
  TAC_READ,       /* read a */
  TAC_RET,        /* return a */
  TAC_BEGIN_FUNC, /* Inicio de função */
  TAC_END_FUNC,   /* Fim de função */
  TAC_CALL,       /* call f */
  TAC_ARG         /* param a */
} tac_op_type;

typedef struct tac {
  tac_op_type type;
  char *res; /* Resultado (t0) ou Destino (L1) */
  char *op1; /* Operando 1 */
  char *op2; /* Operando 2 */

  struct tac *prev;
  struct tac *next;
} tac_t;

tac_t *tac_create(tac_op_type type, char *res, char *op1, char *op2);
tac_t *tac_join(tac_t *l1, tac_t *l2);
void tac_print(tac_t *tac);

char *make_tempo(void);
char *make_label(void);

#endif
