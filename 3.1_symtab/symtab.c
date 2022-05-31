/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object *obj);
void freeScope(Scope *scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab *symtab;
Type *intType;
Type *charType;

/******************* Type utilities ******************************/

Type *makeIntType(void)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type *makeCharType(void)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type *makeArrayType(int arraySize, Type *elementType)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type *duplicateType(Type *type)
{
  // TODO
  Type *dType = (Type *)malloc(sizeof(Type));
  dType->typeClass = type->typeClass;
  dType->arraySize = type->arraySize;
  if (type->elementType != NULL)
  {
    dType->elementType = duplicateType(type->elementType);
  }
  return dType;
}

int compareType(Type *type1, Type *type2)
{
  // TODO
  if (type1->typeClass != type2->typeClass)
  {
    return 0;
  }
  else if (type1->typeClass == TP_INT)
  {
    return 1;
  }
  else if (type1->typeClass == TP_CHAR)
  {
    return 1;
  }
  else if (type1->typeClass == TP_ARRAY)
  {
    if (type1->arraySize != type2->arraySize)
    {
      return 0;
    }
    else
    {
      return compareType(type1->elementType, type2->elementType);
    }
  }
  else
  {
    return 0;
  }
}

void freeType(Type *type)
{
  // TODO
  if (type->typeClass == TP_ARRAY)
  {
    freeType(type->elementType);
    free(type);
  }
  else
  {
    free(type);
  }
}

/******************* Constant utility ******************************/

ConstantValue *makeIntConstant(int i)
{
  // TODO
  ConstantValue *cv = (ConstantValue *)malloc(sizeof(ConstantValue));
  cv->charValue = i;
  cv->type = TP_INT;

  return cv;
}

ConstantValue *makeCharConstant(char ch)
{
  // TODO
  ConstantValue *cv = (ConstantValue *)malloc(sizeof(ConstantValue));
  cv->charValue = ch;
  cv->type = TP_CHAR;

  return cv;
}

ConstantValue *duplicateConstantValue(ConstantValue *v)
{
  // TODO
  ConstantValue *cv = (ConstantValue *)malloc(sizeof(ConstantValue));
  cv->charValue = v->charValue;
  cv->type = TP_CHAR;

  return cv;
}

/******************* Object utilities ******************************/

Scope *createScope(Object *owner, Scope *outer)
{
  Scope *scope = (Scope *)malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object *createProgramObject(char *programName)
{
  Object *program = (Object *)malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes *)malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = createScope(program, NULL);
  symtab->program = program;

  return program;
}

Object *createConstantObject(char *name)
{
  // TODO
  Object *constant = (Object *)malloc(sizeof(Object));
  strcpy(constant->name, name);
  constant->kind = OBJ_CONSTANT;
  constant->constAttrs = (ConstantAttributes *)malloc(sizeof(ConstantAttributes));

  return constant;
}

Object *createTypeObject(char *name)
{
  // TODO
  Object *typeObj = (Object *)malloc(sizeof(Object));
  strcpy(typeObj->name, name);
  typeObj->typeAttrs = (TypeAttributes *)malloc(sizeof(TypeAttributes));
  typeObj->kind = OBJ_TYPE;

  return typeObj;
}

Object *createVariableObject(char *name)
{
  // TODO
  Object *obj = (Object *)malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->varAttrs = (VariableAttributes *)malloc(sizeof(VariableAttributes));
  obj->kind = OBJ_VARIABLE;

  return obj;
}

Object *createFunctionObject(char *name)
{
  // TODO
  Object *func = (Object *)malloc(sizeof(Object));
  strcpy(func->name, name);
  func->kind = OBJ_FUNCTION;
  func->funcAttrs = (FunctionAttributes *)malloc(sizeof(FunctionAttributes));
  func->funcAttrs->scope = createScope(func, symtab->currentScope);

  return func;
}

Object *createProcedureObject(char *name)
{
  // TODO
  Object *proc = (Object *)malloc(sizeof(Object));
  proc->kind = OBJ_PROCEDURE;
  proc->procAttrs = (ProcedureAttributes *)malloc(sizeof(ProcedureAttributes));
  proc->procAttrs->paramList = (ObjectNode *)malloc(sizeof(ObjectNode));
  proc->procAttrs->scope = createScope(proc, symtab->currentScope);

  return proc;
}

Object *createParameterObject(char *name, enum ParamKind kind, Object *owner)
{
  // TODO
  Object *parameter = (Object *)malloc(sizeof(Object));
  strcpy(parameter->name, name);
  parameter->kind = OBJ_PARAMETER;
  parameter->paramAttrs = (ParameterAttributes *)malloc(sizeof(ParameterAttributes));
  parameter->paramAttrs->kind = kind;
  parameter->paramAttrs->type = (Type *)malloc(sizeof(Type));
  // parameter->paramAttrs->type->typeClass = TP_INT;

  // if (kind == PARAM_VALUE)
  // {
  //   parameter->paramAttrs->type = owner->funcAttrs->returnType;
  // }
  // else
  // {
  //   parameter->paramAttrs->type = owner->funcAttrs->paramList->paramAttrs->type;
  // }

  return parameter;
}

void freeObject(Object *obj)
{
  // TODO
  free(obj->name);
  switch (obj->kind)
  {
  case OBJ_CONSTANT:
    free(obj->constAttrs->value);
    free(obj->constAttrs);
    break;
  case OBJ_FUNCTION:
    if (obj->funcAttrs->paramList->next)
    {
      freeObject(obj->funcAttrs->paramList->next->object);
      freeObject(obj->funcAttrs->paramList->next);
    }
    freeObject(obj->funcAttrs->paramList->object);
    freeType(obj->funcAttrs->returnType);
    free(obj->funcAttrs->scope);
    free(obj->funcAttrs);
    break;
  case OBJ_PARAMETER:
    freeType(obj->paramAttrs->type);
    freeObject(obj->paramAttrs->function);
    // TODO: obj->paramAttrs->kind
    break;
  case OBJ_PROCEDURE:
    if (obj->procAttrs->paramList->next)
    {
      freeObject(obj->procAttrs->paramList->next->object);
      freeObject(obj->procAttrs->paramList->next);
    }
    freeObject(obj->procAttrs->paramList->object);
    free(obj->procAttrs->scope);
    free(obj->procAttrs);
    break;
  }
}

void freeScope(Scope *scope)
{
  // TODO
}

void freeObjectList(ObjectNode *objList)
{
  // TODO
}

void freeReferenceList(ObjectNode *objList)
{
  // TODO
}

void addObject(ObjectNode **objList, Object *obj)
{
  ObjectNode *node = (ObjectNode *)malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL)
    *objList = node;
  else
  {
    ObjectNode *n = *objList;
    while (n->next != NULL)
      n = n->next;
    n->next = node;
  }
}

Object *findObject(ObjectNode *objList, char *name)
{
  // TODO
}

/******************* others ******************************/

void initSymTab(void)
{
  Object *obj;
  Object *param;

  symtab = (SymTab *)malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;

  obj = createFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEI");
  param = createParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->procAttrs->paramList), param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEC");
  param = createParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  addObject(&(obj->procAttrs->paramList), param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITELN");
  addObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void)
{
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope *scope)
{
  symtab->currentScope = scope;
}

void exitBlock(void)
{
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object *obj)
{
  if (obj->kind == OBJ_PARAMETER)
  {
    Object *owner = symtab->currentScope->owner;
    switch (owner->kind)
    {
    case OBJ_FUNCTION:
      addObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      addObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }

  addObject(&(symtab->currentScope->objList), obj);
}
