#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/ast.h"

void* safe_alloc(size_t size) {
    void* ptr = calloc(1, size);
    if (!ptr) {
        fprintf(stderr, "Erro fatal: Memoria insuficiente.\n");
        exit(1);
    }
    return ptr;
}

expr_t *ast_expr_literal_int(int value, int line) {
    expr_t *node = (expr_t *)safe_alloc(sizeof(expr_t));
    node->kind = EXPR_LIT_INT;
    node->line = line;
    node->data.i_val = value;
    return node;
}

expr_t *ast_expr_literal_string(char *value, int line) {
    expr_t *node = (expr_t *)safe_alloc(sizeof(expr_t));
    node->kind = EXPR_LIT_STRING; // Ou EXPR_ID dependendo da sua lógica
    node->line = line;
    node->data.s_val = value ? strdup(value) : NULL;
    return node;
}

expr_t *ast_expr_binary(expr_e kind, expr_t *left, expr_t *right, int line) {
    expr_t *node = (expr_t *)safe_alloc(sizeof(expr_t));
    node->kind = kind;
    node->line = line;
    node->data.bin_op.left = left;
    node->data.bin_op.right = right;
    return node;
}

expr_t *ast_expr_call(char *id, expr_t *args, int line) {
    expr_t *node = (expr_t *)safe_alloc(sizeof(expr_t));
    node->kind = EXPR_CALL;
    node->line = line;
    node->data.call.id = id ? strdup(id) : NULL;
    node->data.call.args = args;
    return node;
}

/* Se EXPR_ASSIGN for usado para expressões tipo (x = 10) retornando valor */
expr_t *ast_expr_assign(char *id, expr_t *expr, int line) {
    expr_t *node = (expr_t *)safe_alloc(sizeof(expr_t));
    node->kind = EXPR_ASSIGN;
    node->line = line;
    node->data.assign.id = id ? strdup(id) : NULL;
    node->data.assign.expr = expr;
    node->next = NULL;
    return node;
}

/* --- COMANDOS --- */

cmd_t *ast_cmd_assign(char *id, expr_t *expr, int line) {
    cmd_t *node = (cmd_t *)safe_alloc(sizeof(cmd_t));
    node->kind = CMD_ASSIGN;
    node->line = line;
    node->data.assign.id = id ? strdup(id) : NULL;
    node->data.assign.expr = expr;
    return node;
}

cmd_t *ast_cmd_if(expr_t *cond, cmd_t *body, cmd_t *else_body, int line) {
    cmd_t *node = (cmd_t *)safe_alloc(sizeof(cmd_t));
    node->kind = CMD_IF;
    node->line = line;
    node->data.control.cond = cond;
    node->data.control.body = body;
    node->data.control.else_body = else_body;
    return node;
}

cmd_t *ast_cmd_while(expr_t *cond, cmd_t *body, int line) {
    cmd_t *node = (cmd_t *)safe_alloc(sizeof(cmd_t));
    node->kind = CMD_WHILE;
    node->line = line;
    node->data.control.cond = cond;
    node->data.control.body = body;
    return node;
}

cmd_t *ast_cmd_ret(expr_t *expr, int line) {
    cmd_t *node = (cmd_t *)safe_alloc(sizeof(cmd_t));
    node->kind = CMD_RETURN;
    node->line = line;
    node->data.io_expr.expr = expr;
    return node;
}

cmd_t *ast_cmd_leia(char *id, int line) {
    cmd_t *node = (cmd_t *)safe_alloc(sizeof(cmd_t));
    node->kind = CMD_LEIA;
    node->line = line;
    node->data.io_read.id = id ? strdup(id) : NULL;
    return node;
}

cmd_t *ast_cmd_escreva(expr_t *expr, int line) {
    cmd_t *node = (cmd_t *)safe_alloc(sizeof(cmd_t));
    node->kind = CMD_ESCREVA;
    node->line = line;
    node->data.io_expr.expr = expr;
    return node;
}

cmd_t *ast_cmd_block(block_t *blk, int line) {
    cmd_t *node = (cmd_t *)safe_alloc(sizeof(cmd_t));
    node->kind = CMD_BLOCK;
    node->data.block = blk;
    node->line = line;
    return node;
}

/* --- ESTRUTURAIS --- */

var_decl_t *ast_decl_var(types_t type, char *id, int line) {
    var_decl_t *node = (var_decl_t *)safe_alloc(sizeof(var_decl_t));
    node->type = type;
    node->id = id ? strdup(id) : NULL;
    node->line = line;
    return node;
}

block_t *ast_block(var_decl_t *vars, cmd_t *cmds, int line) {
    block_t *node = (block_t *)safe_alloc(sizeof(block_t));
    node->vars = vars;
    node->cmds = cmds;
    node->line = line;
    return node;
}

param_t *ast_param(types_t type, char *id, int line) {
    param_t *node = (param_t *)safe_alloc(sizeof(param_t));
    node->type = type;
    node->line = line;
    node->id = id ? strdup(id) : NULL;
    return node;
}

func_decl_t *ast_decl_func(types_t type, char *id, param_t *params, block_t *blk, int line) {
    func_decl_t *node = (func_decl_t *)safe_alloc(sizeof(func_decl_t));
    node->ret_type = type;
    node->line = line;
    node->id = id ? strdup(id) : NULL;
    node->block = blk;
    node->params = params;
    return node;
}

global_decl_t *ast_global_var(var_decl_t *var, int line) {
    global_decl_t *node = (global_decl_t *)safe_alloc(sizeof(global_decl_t));
    node->kind = G_VAR;
    node->decl.var = var;
    node->line = line;
    return node;
}

global_decl_t *ast_global_func(func_decl_t *func, int line) {
    global_decl_t *node = (global_decl_t *)safe_alloc(sizeof(global_decl_t));
    node->kind = G_FUNC;
    node->decl.func = func;
    node->line = line;
    return node;
}

program_t *ast_program(global_decl_t *globals, block_t *main_block, int line) {
    program_t *node = (program_t *)safe_alloc(sizeof(program_t));
    node->globals = globals;
    node->main_block = main_block;
    node->line = line;
    return node;
}

/* Função auxiliar para concatenar listas de comandos */
void ast_append_cmd(cmd_t *head, cmd_t *new_node) {
    if (!head) return;
    cmd_t *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}
