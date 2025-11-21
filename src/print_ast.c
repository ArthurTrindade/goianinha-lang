#include <stdio.h>
#include "../include/ast.h"
#include "../include/print_ast.h"

/* Helper para imprimir tipos de dados */
const char *get_type_name(types_t t) {
    switch (t) {
        case T_INT: return "int";
        case T_CAR: return "car";
        case T_VOID: return "void";
        default: return "unknown";
    }
}

/* Helper para imprimir operadores binários */
const char *get_op_symbol(expr_e kind) {
    switch (kind) {
        case EXPR_ADD: return "+";
        case EXPR_SUB: return "-";
        case EXPR_MUL: return "*";
        case EXPR_DIV: return "/";
        case EXPR_AND: return "&&";
        case EXPR_OR:  return "||";
        case EXPR_EQUAL: return "==";
        case EXPR_DIFF:  return "!=";
        case EXPR_LESS:  return "<";
        case EXPR_GREATER: return ">";
        case EXPR_LESS_EQUAL: return "<=";
        case EXPR_GREATER_EQUAL: return ">=";
        default: return "?";
    }
}

void print_expr(expr_t *expr) {
    if (!expr) return;

    switch (expr->kind) {
        case EXPR_LIT_INT:
            printf("%d", expr->data.i_val);
            break;

        case EXPR_LIT_STRING:
            /* Pode ser uma string "abc" ou um identificador "variavel_x" */
            printf("%s", expr->data.s_val);
            break;

        case EXPR_ASSIGN: {
            printf("\nAtribuicao: %s = ", expr->data.assign.id);
            print_expr(expr->data.assign.expr);
            printf("\n");
            break;
        }

        case EXPR_CALL:
            printf("%s(", expr->data.call.id);
            expr_t *arg = expr->data.call.args;
            while (arg) {
                print_expr(arg);
                if (arg->next) printf(", ");
                arg = arg->next;
            }
            printf(")");
            break;

        /* Operações Binárias Agrupadas */
        case EXPR_ADD: case EXPR_SUB: case EXPR_MUL: case EXPR_DIV:
        case EXPR_AND: case EXPR_OR:  case EXPR_EQUAL: case EXPR_DIFF:
        case EXPR_LESS: case EXPR_GREATER:
        case EXPR_LESS_EQUAL: case EXPR_GREATER_EQUAL:
            printf("(");
            print_expr(expr->data.bin_op.left);
            printf(" %s ", get_op_symbol(expr->kind));
            print_expr(expr->data.bin_op.right);
            printf(")");
            break;

        /* Operações Unárias */
        case EXPR_NOT: /* Exemplo se adicionarmos ! */
             printf("!");
             print_expr(expr->data.call.args);
             break;

        default:
            printf("Expr?");
    }
}

/* Declaração antecipada necessária para recursão mútua */
void print_block(block_t *block);

void print_cmd(cmd_t *cmd) {
    if (!cmd) return;

    printf("Linha %d: ", cmd->line);

    switch (cmd->kind) {
        case CMD_ASSIGN:
            printf("Atribuicao: %s = ", cmd->data.io_expr.expr->data.s_val);
            print_expr(cmd->data.io_expr.expr);
            printf("\n");
            break;

        case CMD_IF:
            printf("Se: ");
            print_expr(cmd->data.control.cond);
            printf("\n  Entao:\n");
            print_cmd(cmd->data.control.body); // Imprime o bloco ou comando único

            if (cmd->data.control.else_body) {
                printf("  Senao:\n");
                print_cmd(cmd->data.control.else_body);
            }
            break;

        case CMD_WHILE:
            printf("Enquanto: ");
            print_expr(cmd->data.control.cond);
            printf("\n  Faca:\n");
            print_cmd(cmd->data.control.body);
            break;

        case CMD_RETURN:
            printf("Retorne: ");
            print_expr(cmd->data.io_expr.expr);
            printf("\n");
            break;

        case CMD_LEIA:
            printf("Leia: %s\n", cmd->data.io_read.id);
            break;

        case CMD_ESCREVA:
            printf("Escreva: ");
            print_expr(cmd->data.io_expr.expr);
            printf("\n");
            break;

        case CMD_BLOCK:
            printf("--- Inicio Bloco ---\n");
            print_block(cmd->data.block);
            printf("--- Fim Bloco ---\n");
            break;

        case CMD_EXPR:
             printf("Expr Isolada: ");
             print_expr(cmd->data.io_expr.expr); /* Reutilizando io_expr para expr geral */
             printf("\n");
             break;

        default:
            printf("Comando desconhecido\n");
    }
}

void print_var_decl(var_decl_t *vars) {
    while (vars) {
        printf("  [VAR] %s %s (Linha %d)\n",
               get_type_name(vars->type),
               vars->id,
               vars->line);
        vars = vars->next;
    }
}

void print_block(block_t *block) {
    if (!block) return;

    /* 1. Imprimir Variáveis Locais */
    if (block->vars) {
        printf("  Declaracoes Locais:\n");
        print_var_decl(block->vars);
    }

    /* 2. Imprimir Lista de Comandos */
    cmd_t *curr_cmd = block->cmds;
    while (curr_cmd) {
        print_cmd(curr_cmd);
        curr_cmd = curr_cmd->next; /* Lista intrusiva: o comando sabe o próximo */
    }
}

void print_params(param_t *params) {
    printf("(");
    while (params) {
        printf("%s %s", get_type_name(params->type), params->id);
        if (params->next) printf(", ");
        params = params->next;
    }
    printf(")");
}

void print_program(program_t *prog) {
    if (!prog) {
        printf("Programa Vazio (NULL)\n");
        return;
    }

    printf("=== AST START ===\n");

    /* 1. Declarações Globais */
    global_decl_t *glob = prog->globals;
    while (glob) {
        if (glob->kind == G_VAR) {
            printf("[GLOBAL VAR] ");
            print_var_decl(glob->decl.var);
        } else if (glob->kind == G_FUNC) {
            func_decl_t *f = glob->decl.func;
            printf("\n[FUNCAO] %s %s", get_type_name(f->ret_type), f->id);
            print_params(f->params);
            printf("\n");
            print_block(f->block);
        }
        glob = glob->next;
    }

    /* 2. Bloco Principal */
    if (prog->main_block) {
        printf("\n=== MAIN BLOCK ===\n");
        print_block(prog->main_block);
    }

    printf("=== AST END ===\n");
}
