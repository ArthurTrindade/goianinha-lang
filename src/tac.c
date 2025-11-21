#include "../include/tac.h"
#include <stdio.h>
#include <string.h>
#include "../include/ast.h"

// TODO: Trocar para uma lista duplamente encadedae com ponteiro para o final

tac_t *tac_create(tac_op_type type, char *res, char *op1, char *op2) {
  tac_t *new_node = safe_alloc(sizeof(tac_t));
  new_node->type = type;
  new_node->res = res ? strdup(res) : NULL;
  new_node->op1 = op1 ? strdup(op1) : NULL;
  new_node->op2 = op1 ? strdup(op2) : NULL;
  return new_node;
}

/* junta duas listas, l1 <- l2 */
tac_t *tac_join(tac_t *l1, tac_t *l2) {
  if (!l1)
    return l2;
  if (!l2)
    return l1;

  tac_t *current = l1;
  while (current->next) {
    current = current->next;
  }

  current->next = l2;
  l2->prev = current;

  return l1;
}

void tac_print(tac_t *tac) {
  if (!tac)
    return;

  static char *names[] = {"SYMBOL",     "ADD",      "SUB",  "MUL",
                          "DIV",        "MOVE",     "IFZ",  "LABEL",
                          "JMP",        "PRINT",    "READ", "RET",
                          "BEGIN_FUNC", "END_FUNC", "CALL", "ARG"};

  while (tac != NULL) {
    fprintf(stdout, "TAC(%s, %s, %s, %s)\n", names[tac->type],
            tac->res ? tac->res : "-", tac->op1 ? tac->op2 : "-",
            tac->op2 ? tac->op2 : "-");
    tac = tac->next;
  }
}

char *make_temp(void) {
    static int serial = 0;
    char buffer[32];
    sprintf(buffer, "t%d", serial++);
    return strdup(buffer);
}

char *make_label(void) {
    static int serial = 0;
    char buffer[32];
    sprintf(buffer, ".L%d", serial++);
    return strdup(buffer);
}
