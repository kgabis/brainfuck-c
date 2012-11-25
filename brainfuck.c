/*
 Brainfuck-C ( http://github.com/kgabis/brainfuck-c )
 Copyright (c) 2012 Krzysztof Gabis
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

#include <stdio.h>

#define OP_END          0
#define OP_INC_DP       1
#define OP_DEC_DP       2
#define OP_INC_VAL      3
#define OP_DEC_VAL      4
#define OP_OUT          5
#define OP_IN           6
#define OP_JMP_FWD      7
#define OP_JMP_BCK      8

#define SUCCESS         1
#define FAILURE         0

#define PROGRAM_SIZE    4096
#define STACK_SIZE      512
#define DATA_SIZE       40000

#define SET_OP(A, OP)   (A = (A & 0) | OP)
#define GET_OP(A)       (A & 0xf)
#define SET_JMP(A, POS) (A = A | (POS << 4))
#define GET_JMP(A)      (A >> 4)

#define STACK_PUSH(A)   (STACK[SP++] = A)
#define STACK_POP()     (STACK[--SP])
#define STACK_EMPTY()   (SP == 0)
#define STACK_FULL()    (SP == STACK_SIZE)

static unsigned int PROGRAM[PROGRAM_SIZE];
static unsigned int STACK[STACK_SIZE];
static unsigned int SP = 0;

int compile_bf() {
    unsigned int pc = 0, jmp_pc;
    int c;
    while ((c = getchar()) != EOF && pc < PROGRAM_SIZE) {
        switch (c) {
            case '>': SET_OP(PROGRAM[pc], OP_INC_DP); break;
            case '<': SET_OP(PROGRAM[pc], OP_DEC_DP); break;
            case '+': SET_OP(PROGRAM[pc], OP_INC_VAL); break;
            case '-': SET_OP(PROGRAM[pc], OP_DEC_VAL); break;
            case '.': SET_OP(PROGRAM[pc], OP_OUT); break;
            case ',': SET_OP(PROGRAM[pc], OP_IN); break;
            case '[':
                SET_OP(PROGRAM[pc], OP_JMP_FWD);
                if (STACK_FULL()) {
                    return FAILURE;
                }
                STACK_PUSH(pc);
                break;
            case ']':
                if (STACK_EMPTY()) {
                    return FAILURE;
                }
                jmp_pc = STACK_POP();
                SET_OP(PROGRAM[pc], OP_JMP_BCK);
                SET_JMP(PROGRAM[pc], jmp_pc);
                SET_JMP(PROGRAM[jmp_pc], pc);
                break;
            default: pc--; break;
        }
        pc++;
    }
    if (!STACK_EMPTY() || pc == PROGRAM_SIZE) {
        return FAILURE;
    }
    SET_OP(PROGRAM[pc], OP_END);
    return SUCCESS;
}

int execute_bf() {
    unsigned int data[DATA_SIZE], ptr = DATA_SIZE, pc = 0;
    while (--ptr) { data[ptr] = 0; }
    while (GET_OP(PROGRAM[pc]) != OP_END && ptr < DATA_SIZE) {
        switch (GET_OP(PROGRAM[pc])) {
            case OP_INC_DP: ptr++; break;
            case OP_DEC_DP: ptr--; break;
            case OP_INC_VAL: data[ptr]++; break;
            case OP_DEC_VAL: data[ptr]--; break;
            case OP_OUT: putchar(data[ptr]); break;
            case OP_IN: data[ptr] = (unsigned int)getchar(); break;
            case OP_JMP_FWD: if(!data[ptr]) { pc = GET_JMP(PROGRAM[pc]); } break;
            case OP_JMP_BCK: if(data[ptr]) { pc = GET_JMP(PROGRAM[pc]); } break;
            default: return FAILURE;
        }
        pc++;
    }
    if (ptr == DATA_SIZE) {
        return FAILURE;
    }
    return SUCCESS;
}

int main(int argc, const char * argv[])
{
    int status;
    status = compile_bf();
    if (status == SUCCESS) {
        status = execute_bf();
    }
    if (status == FAILURE) {
        fprintf(stderr, "Error!\n");
        return 1;
    }
    printf("\n");
    return 0;
}


