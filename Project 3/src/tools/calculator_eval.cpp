// src/tools/calculator_eval.cpp — YOU IMPLEMENT (spec §4.6).
//
// calc_evaluate: evaluate a well-formed token list with two stacks
// (one for numbers, one for operators and '('). See spec §4.6 and the
// worked trace in Appendix C. std::stack or std::vector are both fine.

#include "tools/calculator.h"
#include <stack>

bool calc_evaluate(const std::vector<CalcToken>& tokens, double* out,
                   std::string* error) {
    std::stack<double> values;   //creates a stack for numbers
    std::stack<CalcToken::Kind> ops;  //creates a stack for operations
    for (std::size_t i = 0; i < tokens.size(); i++) {  //runs through and processess all the new tokens coming in
        if (tokens[i].kind == CalcToken::Kind::Number) {  //pushes tokens number value onto the vlaues stack
            values.push(tokens[i].value);
        }
        else if (tokens[i].kind == CalcToken::Kind::LParen) {
            ops.push(tokens[i].kind);   //goes ahead and pushes l parenthesis token onto stack
        }
        else if (tokens[i].kind == CalcToken::Kind::RParen) {  //if a r parenthesis comes in it will check for a left one
            while ((ops.empty() == false) && ops.top() != CalcToken::Kind::LParen) { // and evaluate inside components
                if (values.size() < 2) { //checks if enough numbers are available for operation
                    *error = "not enough nums for operation";
                    return false;
                }
                double val2 = values.top(); //steals the top two vals and operation, can be important for order specific operations like sub and div
                values.pop();
                double val1 = values.top();
                values.pop();
                CalcToken::Kind symbol = ops.top();
                ops.pop();
                double result;
                if (symbol == CalcToken::Kind::Plus) { //completes operation inside the parenthesis
                    result = val1 + val2;
                }
                else if (symbol == CalcToken::Kind::Minus) {
                    result = val1 - val2;
                }
                else if (symbol == CalcToken::Kind::Times) {
                    result = val1 * val2;
                }
                else if (symbol == CalcToken::Kind::Divide) {
                    if (val2 == 0.0) {
                        *error = "division by zero";
                        return false;
                    }
                    result = val1 / val2;
                }
                else {
                    *error = "no/non-useable operator symbol"; //if there are no legal operator symbols
                    return false;
                }
                values.push(result);
            }
            if (ops.empty() == true) {  //checks to see if stack is empty before l parenthesis is found
                *error = "left parenthesis is missing/ggone";
                return false;
            }
            ops.pop(); //removes l parenthesis
        }
        else {
            int newPriority = 1;         //assigns priority of the operation according to pemdas
            if ((tokens[i].kind == CalcToken::Kind::Times) ||   //if mult or div its given a higher priority
                (tokens[i].kind == CalcToken::Kind::Divide)) {
                newPriority = 2;
                }
            while ((ops.empty() == false) && (ops.top() != CalcToken::Kind::LParen)) {  //performs operation for ops with higher priority
                int waitPriority = 1;
                if ((ops.top() == CalcToken::Kind::Times) || (ops.top() == CalcToken::Kind::Divide)) {
                    waitPriority = 2;
                }
                if (waitPriority < newPriority) { //leaves a lower priority op waiting in stack
                    break;
                }
                if (values.size() < 2) {
                    *error = "not enough nums for operation";
                    return false;
                }
                double val2 = values.top();
                values.pop();
                double val1 = values.top();
                values.pop();
                CalcToken::Kind symbol = ops.top();
                ops.pop();
                double result;
                if (symbol == CalcToken::Kind::Plus) {
                    result = val1 + val2;
                }
                else if (symbol == CalcToken::Kind::Minus) {
                    result = val1 - val2;
                }
                else if (symbol == CalcToken::Kind::Times) {
                    result = val1 * val2;
                }
                else if (symbol == CalcToken::Kind::Divide) {
                    if (val2 == 0.0) {
                        *error = "division by zero";
                        return false;
                    }
                    result = val1 / val2;
                }
                else {
                    *error = "no/non-useable operator symbol";
                    return false;
                }
                values.push(result);
            }
            ops.push(tokens[i].kind);
        }
    }
    while (ops.empty() == false) {  //finish the problem after all tokens have been input.
        if (values.size() < 2) {
            *error = "not enough nums for operation";
            return false;
        }
        double val2 = values.top();
        values.pop();
        double val1 = values.top();
        values.pop();
        CalcToken::Kind symbol = ops.top();
        ops.pop();
        double result;
        if (symbol == CalcToken::Kind::Plus) {
            result = val1 + val2;
        }
        else if (symbol == CalcToken::Kind::Minus) {
            result = val1 - val2;
        }
        else if (symbol == CalcToken::Kind::Times) {
            result = val1 * val2;
        }
        else if (symbol == CalcToken::Kind::Divide) {
            if (val2 == 0.0) {
                *error = "division by zero";
                return false;
            }
            result = val1 / val2;
        }
        else {
            *error = "no/non-useable operator symbol";
            return false;
        }
        values.push(result);
    }
    if (values.size() != 1) {
        *error = "invalid expression";
        return false;
    }
    *out = values.top();  //writes answer to out
    return true;
}
