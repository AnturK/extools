#pragma once
#include "instruction.h"
#include "context.h"

ADD_INSTR(END)
ADD_INSTR_JUMP(JMP, 1)
ADD_INSTR_JUMP(JZ, 1)
ADD_INSTR(RET)
ADD_INSTR_CUSTOM(CALL)
ADD_INSTR_CUSTOM(CALLNR)
ADD_INSTR_ARG(CALLPATH, 1)
ADD_INSTR(CALLPARENT)
ADD_INSTR_CUSTOM(CALLGLOB)
ADD_INSTR_CUSTOM(CALL_GLOBAL_ARGLIST)
ADD_INSTR_ARG(CALLNAME, 1)
ADD_INSTR_JUMP(JMP2, 1)
ADD_INSTR_JUMP(JNZ, 1)
ADD_INSTR_JUMP(JNZ2, 1)
ADD_INSTR_JUMP(JZ2, 1)
ADD_INSTR_ARG(CALL_LIB, 1)
ADD_INSTR_ARG(CALL_LIB_ARGLIST, 1)
ADD_INSTR_JUMP(JMP_OR, 1)
ADD_INSTR_JUMP(JMP_AND, 1)
ADD_INSTR_CUSTOM(SWITCH)