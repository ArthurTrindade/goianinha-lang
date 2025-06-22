#include <stdio.h> // Para fprintf, snprintf
#include <stdbool.h> // Para tipo bool

// Assumindo que você tem essas declarações em outros arquivos ou no topo deste:
// extern void report_semantic_error(int line, const char *message);
// extern const char* type_to_string(types_t type); // Converte types_t para string
// extern const char* expr_kind_to_string(expr_e kind); // Converte expr_e para string

// Função auxiliar para verificar compatibilidade de tipos (exemplo simples)
// Você pode expandir isso para regras mais complexas de conversão implícita.
static bool are_types_compatible(types_t expected, types_t found) {
    if (expected == found) return true;
    // Exemplo: int pode ser promovido para float em algumas operações
    // if (expected == T_FLOAT && found == T_INT) return true;
    // Outras regras específicas da sua linguagem
    return false;
}

// Declaração forward para funções que semantic_expr pode chamar
types_t semantic_expr(env_t current_env, expr_t *node); // Define o tipo de retorno para semantic_expr

types_t semantic_expr(env_t current_env, expr_t *node) {
    if (node == NULL) return T_UNKNOWN;

    char error_msg[256]; // Buffer para mensagens de erro formatadas

    switch (node->kind) {
        case EXPR_ID: {
            if (node->id == NULL) {
                report_semantic_error(node->line, "Identificador de expressão é NULL.");
                return T_UNKNOWN;
            }
            symbol_t *sym = symbol_search(current_env, node->id);
            if (sym == NULL) {
                snprintf(error_msg, sizeof(error_msg), "Identificador '%s' não encontrado.", node->id);
                report_semantic_error(node->line, error_msg);
                return T_UNKNOWN;
            }

            if (sym->symbol_type == T_VAR || sym->symbol_type == T_PARAM) {
                return sym->data_type;
            } else if (sym->symbol_type == T_FUNCTION) {
                // Aqui estamos tratando EXPR_ID que pode ser uma chamada de função
                // A AST precisa distinguir a chamada de função de um ID simples.
                // Sua AST tem 'expr_t->expr_list' para isso.

                if (node->expr_list != NULL) { // É uma chamada de função
                    int args_provided = 0;
                    expr_list_t *current_arg_expr = node->expr_list;
                    while(current_arg_expr) {
                        args_provided++;
                        current_arg_expr = current_arg_expr->next;
                    }

                    if (args_provided != list_size(sym->params)) {
                        snprintf(error_msg, sizeof(error_msg), 
                                 "Número incorreto de argumentos para a função '%s'. Esperado: %d, Recebido: %d", 
                                 node->id, list_size(sym->params), args_provided);
                        report_semantic_error(node->line, error_msg);
                    }

                    current_arg_expr = node->expr_list;
                    node_t *expected_param_node = list_head(sym->params);
                    int arg_idx = 0;
                    while (current_arg_expr && expected_param_node) {
                        types_t provided_type = semantic_expr(current_env, current_arg_expr->expr);
                        symbol_t *expected_param_sym = (symbol_t*)list_data(expected_param_node);
                        
                        if (!are_types_compatible(expected_param_sym->data_type, provided_type)) {
                            snprintf(error_msg, sizeof(error_msg), 
                                     "Tipo de argumento %d incompatível na chamada da função '%s'. Esperado: %s, Recebido: %s", 
                                     arg_idx, node->id, 
                                     type_to_string(expected_param_sym->data_type), 
                                     type_to_string(provided_type));
                            report_semantic_error(current_arg_expr->line, error_msg);
                        }
                        current_arg_expr = current_arg_expr->next;
                        expected_param_node = list_next(expected_param_node);
                        arg_idx++;
                    }
                    return sym->data_type; // Retorna o tipo de retorno da função
                } else {
                    // É um ID de função, mas não uma chamada (e.g., usado como uma variável)
                    snprintf(error_msg, sizeof(error_msg), "Uso de função '%s' sem chamada (parênteses).", node->id);
                    report_semantic_error(node->line, error_msg);
                    return T_UNKNOWN;
                }
            }
            return T_UNKNOWN; // Tipo de símbolo não esperado para expressão
        }

        case EXPR_INT: return T_INT; // Literal inteiro
        case EXPR_CHAR: return T_CAR; // Literal char
        // Adicione outros literais se sua linguagem os suporta, e.g., EXPR_FLOAT, EXPR_BOOL.

        case EXPR_ADD:   // +
        case EXPR_SUB:   // -
        case EXPR_MUL:   // *
        case EXPR_DIV: { // /
            types_t left_type = semantic_expr(current_env, node->left);
            types_t right_type = semantic_expr(current_env, node->right);

            if (left_type == T_UNKNOWN || right_type == T_UNKNOWN) {
                return T_UNKNOWN; // Erro já reportado nos operandos
            }

            // Para operadores aritméticos, geralmente esperamos tipos numéricos.
            // Aqui, um exemplo simples que só aceita inteiros e retorna inteiro.
            if (left_type == T_INT && right_type == T_INT) {
                return T_INT;
            }
            // Se sua linguagem suporta floats, você expandiria isso:
            // if ((left_type == T_INT && right_type == T_FLOAT) ||
            //     (left_type == T_FLOAT && right_type == T_INT) ||
            //     (left_type == T_FLOAT && right_type == T_FLOAT)) {
            //     return T_FLOAT;
            // }

            snprintf(error_msg, sizeof(error_msg), 
                     "Tipos incompatíveis para operação aritmética '%s'. Tipos: %s e %s", 
                     expr_kind_to_string(node->kind), // Requer a função expr_kind_to_string
                     type_to_string(left_type), type_to_string(right_type));
            report_semantic_error(node->line, error_msg);
            return T_UNKNOWN;
        }

        case EXPR_EQUAL:   // ==
        case EXPR_DIFF:    // !=
        case EXPR_LESS:    // <
        case EXPR_LESS_EQUAL: // <=
        case EXPR_GREATER: // >
        case EXPR_GREATER_EQUAL: { // >=
            types_t left_type = semantic_expr(current_env, node->left);
            types_t right_type = semantic_expr(current_env, node->right);

            if (left_type == T_UNKNOWN || right_type == T_UNKNOWN) {
                return T_UNKNOWN;
            }

            // Operadores relacionais geralmente esperam tipos compatíveis e retornam um booleano.
            // Exemplo: INT com INT, CHAR com CHAR.
            if (are_types_compatible(left_type, right_type) || are_types_compatible(right_type, left_type)) {
                return T_BOOL; // O resultado de uma comparação é um booleano
            }

            snprintf(error_msg, sizeof(error_msg), 
                     "Tipos incompatíveis para operação relacional '%s'. Tipos: %s e %s", 
                     expr_kind_to_string(node->kind), 
                     type_to_string(left_type), type_to_string(right_type));
            report_semantic_error(node->line, error_msg);
            return T_UNKNOWN;
        }

        case EXPR_OR:   // ||
        case EXPR_AND: { // &&
            types_t left_type = semantic_expr(current_env, node->left);
            types_t right_type = semantic_expr(current_env, node->right);

            if (left_type == T_UNKNOWN || right_type == T_UNKNOWN) {
                return T_UNKNOWN;
            }

            // Operadores lógicos esperam tipos booleanos e retornam booleano.
            // Se sua linguagem permite inteiros como booleanos (0 = false, !=0 = true),
            // você precisaria expandir a regra de compatibilidade.
            if (left_type == T_BOOL && right_type == T_BOOL) {
                return T_BOOL;
            }
            // Ex: if ( (left_type == T_BOOL || left_type == T_INT) && (right_type == T_BOOL || right_type == T_INT) ) return T_BOOL;

            snprintf(error_msg, sizeof(error_msg), 
                     "Tipos incompatíveis para operação lógica '%s'. Esperado: Booleano. Tipos: %s e %s", 
                     expr_kind_to_string(node->kind), 
                     type_to_string(left_type), type_to_string(right_type));
            report_semantic_error(node->line, error_msg);
            return T_UNKNOWN;
        }
        
        case EXPR_NOT:   // ! (Operador unário NOT)
        case EXPR_MINUS: { // - (Operador unário de negação)
            types_t operand_type = semantic_expr(current_env, node->left); // Operadores unários geralmente usam 'left'

            if (operand_type == T_UNKNOWN) {
                return T_UNKNOWN;
            }

            if (node->kind == EXPR_NOT) {
                // Operador NOT (!) espera booleano e retorna booleano.
                if (operand_type == T_BOOL) {
                    return T_BOOL;
                }
                snprintf(error_msg, sizeof(error_msg), 
                         "Tipo incompatível para operador unário 'NOT'. Esperado: Booleano. Recebido: %s", 
                         type_to_string(operand_type));
                report_semantic_error(node->line, error_msg);
                return T_UNKNOWN;
            } else if (node->kind == EXPR_MINUS) {
                // Operador unário de negação (-) espera um tipo numérico e retorna o mesmo tipo.
                if (operand_type == T_INT /* || operand_type == T_FLOAT */) {
                    return operand_type;
                }
                snprintf(error_msg, sizeof(error_msg), 
                         "Tipo incompatível para operador unário de negação. Esperado: Numérico. Recebido: %s", 
                         type_to_string(operand_type));
                report_semantic_error(node->line, error_msg);
                return T_UNKNOWN;
            }
            break; // Se chegar aqui, tipo de operador unário não tratado
        }

        case EXPR_ASSIGN: { // Atribuição (e.g., a = b + c)
            // O lado esquerdo (L-value) deve ser um identificador modificável (variável ou parâmetro)
            // e o lado direito (R-value) é a expressão que será atribuída.
            if (node->left == NULL || node->left->kind != EXPR_ID || node->left->id == NULL) {
                report_semantic_error(node->line, "Lado esquerdo da atribuição não é um identificador válido.");
                return T_UNKNOWN;
            }
            symbol_t *lhs_sym = symbol_search(current_env, node->left->id);
            if (lhs_sym == NULL || (lhs_sym->symbol_type != T_VAR && lhs_sym->symbol_type != T_PARAM)) {
                snprintf(error_msg, sizeof(error_msg), 
                         "Atribuição a identificador '%s' que não é uma variável ou parâmetro.", node->left->id);
                report_semantic_error(node->line, error_msg);
                return T_UNKNOWN;
            }

            types_t rhs_type = semantic_expr(current_env, node->right); // Analisa o lado direito
            if (rhs_type == T_UNKNOWN) {
                return T_UNKNOWN; // Erro já reportado no lado direito
            }

            // Verifica compatibilidade de tipos: o tipo do lado direito deve ser compatível com o do lado esquerdo.
            if (!are_types_compatible(lhs_sym->data_type, rhs_type)) {
                snprintf(error_msg, sizeof(error_msg), 
                         "Tipos incompatíveis na atribuição. Variável '%s' (tipo: %s) recebe tipo: %s", 
                         node->left->id, type_to_string(lhs_sym->data_type), type_to_string(rhs_type));
                report_semantic_error(node->line, error_msg);
                return T_UNKNOWN;
            }
            return lhs_sym->data_type; // O tipo da expressão de atribuição é o tipo do L-value
        }

        // EXPR_BIN_OP e EXPR_UN_OP são categorias mais abstratas.
        // Se seus operadores específicos (ADD, SUB, NOT, MINUS, etc.) são diretamente os "kinds" do nó,
        // então EXPR_BIN_OP e EXPR_UN_OP podem não ser necessários como "kinds" de nó, mas sim como tipos de operadores.
        // Se eles são tipos de nó genéricos que depois têm um 'op_kind' dentro, então a lógica é diferente.
        // Pelo seu enum, parece que ADD, SUB, etc., são os próprios 'kinds'.
        case EXPR_BIN_OP: // Tipo de nó genérico para operadores binários, pode ser redundante se ADD, SUB etc são os kinds diretos
        case EXPR_UN_OP: // Tipo de nó genérico para operadores unários
            report_semantic_error(node->line, "Tipo de nó de operador genérico usado. Deve ser um operador específico (EXPR_ADD, EXPR_NOT, etc.).");
            return T_UNKNOWN;

        default:
            snprintf(error_msg, sizeof(error_msg), 
                     "Tipo de expressão desconhecido ou não implementado na análise semântica: %s", 
                     expr_kind_to_string(node->kind)); // Tente converter o kind para string
            report_semantic_error(node->line, error_msg);
            return T_UNKNOWN;
    }
}