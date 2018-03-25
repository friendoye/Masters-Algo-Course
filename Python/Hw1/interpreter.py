#!/usr/bin/env python2
# coding=utf-8
from enum import Enum

class Literal(Enum):
    NONE = 0
    NUMBER = 1
    OPERATOR = 2

def print_debug(text):
    pass

def split_on_token(expr):
    tokens = []
    current_state = Literal.NONE
    current_token = ""
    for i in range(len(expr)):
        c = expr[i]
        if c == " ":
            current_state = Literal.NONE
            tokens.append(prepare_token(current_token))
            current_token = ""
            continue
        elif c in ["(", ")"]:
            current_state = Literal.NONE
            tokens.append(prepare_token(current_token))
            tokens.append(prepare_token(c))
            current_token = ""
            continue
        elif c.isdigit() or c == ".":
            if current_state != Literal.NUMBER:
                if current_token[-1:] not in ["-", "."]:
                    tokens.append(prepare_token(current_token))
                    current_token = ""
                current_state = Literal.NUMBER
            current_token += c
        else:
            if current_state != Literal.OPERATOR:
                tokens.append(prepare_token(current_token))
                current_token = ""
            current_state = Literal.OPERATOR
            current_token += c
    tokens.append(prepare_token(current_token))
    return filter(lambda s: s not in ["", " "], tokens)

def prepare_token(token):
    try:
        if "." in token:
            return float(token)
        else:
            return int(token)
    except:
        return token

def convert_from_infix_to_polish_split(expr):
    operators_priority = {"(": 0, "+": 1, "-": 1, "*": 2, "/": 2, "**": 3}
    # If operator isn't right-associated -> it's left-associated
    supported_right_assoc_operators = ["**"]

    tokens = split_on_token(expr)
    tokens.reverse()

    output_queue = []
    operator_stack = []
    while len(tokens) > 0:
        token = tokens.pop()
        if isinstance(token, (int, float)):
            print_debug("Number: %d" % token)
            output_queue.append(token)
        elif token == "(":
            print_debug("Token (")
            operator_stack.append("(")
        elif token == ")":
            print_debug("Token )")
            while len(operator_stack) > 0 and operator_stack[-1] != "(":
                stack_token = operator_stack.pop()
                output_queue.append(stack_token)
            if len(operator_stack) == 0:
                raise RuntimeError("Found closed, but not opened parenthesis")
            operator_stack.pop()
        elif token in operators_priority.keys():
            print_debug("Operator: " + token)
            while len(operator_stack) > 0:
                top_operator = operator_stack[-1]
                if operators_priority[token] <= operators_priority[top_operator] \
                        and token not in supported_right_assoc_operators:
                    output_queue.append(operator_stack.pop())
                if operators_priority[token] < operators_priority[top_operator] \
                        and token in supported_right_assoc_operators:
                    output_queue.append(operator_stack.pop())
                else:
                    break
            operator_stack.append(token)
        else:
            raise ValueError("Expression contains unknown token: %s" % token)

    while len(operator_stack) > 0:
        operator = operator_stack.pop()
        if operator == "(":
            raise RuntimeError("Found opened, but not closed parenthesis" % token)
        output_queue.append(operator)

    return " ".join(map(lambda x: str(x), output_queue))

def compute_polish_expr(polish_expr):
    operators_action = {"+":  lambda x1, x2: x1 + x2,
                        "-":  lambda x1, x2: x1 - x2,
                        "*":  lambda x1, x2: x1 * x2,
                        "/":  lambda x1, x2: float(x1) / x2,
                        "**": lambda x1, x2: x1 ** x2}

    tokens = split_on_token(polish_expr)
    tokens.reverse()

    numbers_stack = []
    while len(tokens) > 0:
        token = tokens.pop()
        if isinstance(token, (int, float)):
            print_debug("Digit: %d" % token)
            numbers_stack.append(token)
        elif token in operators_action.keys():
            print_debug("Token: " + token)
            arg2 = numbers_stack.pop()
            arg1 = numbers_stack.pop()
            result = operators_action[token](arg1, arg2)
            numbers_stack.append(result)
        else:
            raise RuntimeError("Operator '%s' is not supported!" % token)

    if len(numbers_stack) != 1:
        raise RuntimeError("Polish expr was build incorrectly!")

    return numbers_stack[0]

def eval_math_expr(expr):
    print_debug("Expr: " + expr)
    print_debug("Split on tokens: %s" % split_on_token(expr))
    polish_expr = convert_from_infix_to_polish_split(expr)
    print_debug("Polish notation: " + polish_expr)
    result = compute_polish_expr(polish_expr)
    print_debug("Our result: %s" % repr(result))
    print_debug("Eval result: %s" % repr(eval(expr)))
    return result

if __name__ == "__main__":
    # expr = "3 + 4 * 2 / ( 1 - 5 ) ** 2 ** 3"
    # expr = "13+24*(15+26.0*(14+265*(176+ -2546)))"
    # expr = "2 * (3 + 5)"
    print "Hello! This is math expression interpreter. Enter 'exit' to terminate program."
    while True:
        expr = raw_input()
        try:
            if (expr == "exit"):
                break
            print "Result: %s" % eval_math_expr(expr)
        except Exception as e:
            print "Error(%s)" % e.message
            print "Please, try again."

