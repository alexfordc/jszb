#ifndef TG_INDICAOTR_INDICATORS_H
#define TG_INDICAOTR_INDICATORS_H

#include <stdint.h>

namespace tg {

static const char TYPE_INT = 1;
static const char TYPE_DOUBLE = 2;
static const char TYPE_ARRAY = 3;
struct Value {
	bool isOwnMem;
	char type;
	union {
		int64_t i;
		double f;
		double *values;
	};
	int size; /* ��С */
	int capacity; /* ���� */
	/* values���������һ��Ԫ�صı��,������Ԫ�صı��
	 * ��Ŵ�1��ʼ��ʹ�ñ������ʶԪ�أ�ԭ���ڣ�����size=5000��no���Ե�10000 */
	int no;
};

Value *valueNew();
void valueFree(Value *v);

bool valueExtend(Value *v, int capacity);

double valueGet(const Value *v, int i);
void valueSet(Value *v, int i, double f);

int isValueValid(double f); /* ֵ�Ƿ���Ч */

struct Quote {
	Value *open;
	Value *high;
	Value *low;
	Value *close;
};

/* ע�����������OPEN,CLOSE��;
 * ע�ắ��������MA��SMA�� */
typedef Value *(*ValueFn)(void *parser, int argc, const Value **args, Value *R);
int registerVariable(const char *name, ValueFn fn);
ValueFn findVariable(const char *name);

int registerFunction(const char *name, ValueFn fn);
ValueFn findFunction(const char *name);

/* �ѳ��õ�һЩ�����ͺ���(��CLOSE,MA��)��ʼ�� */
void indicatorInit();
/* �ͷ���Դ */
void indicatorShutdown();

/* ����:������Value��ʱ��,�ڴ������???
 * �������
 * 	RSI
		LC:=REF(CLOSE,1);
		RSI1:SMA(MAX(CLOSE-LC,0),N1,1)/SMA(ABS(CLOSE-LC),N1,1)*100;
 * ���ʹ���ڴ������С??? 
 */
 
Value *OPEN(void *parser);
Value *HIGH(void *parser);
Value *LOW(void *parser);
Value *CLOSE(void *parser);

/* ��������:�Ӽ��˳� */
Value *ADD(const Value *X, const Value *Y, Value *R);
Value *SUB(const Value *X, const Value *Y, Value *R);
Value *MUL(const Value *X, const Value *Y, Value *R);
Value *DIV(const Value *X, const Value *Y, Value *R);

/* R:=REF(X,N); */
Value *REF(const Value *X, int N, Value *R);

/* R:=MAX(X,M) */
Value *MAX(const Value *X, double M, Value *R);

/* R:=ABS(X) */
Value *ABS(const Value *X, Value *R);

/* R:=HHV(X, N) */
Value *HHV(const Value *X, int N, Value *R);

/* R:=LLV(X, N) */
Value *LLV(const Value *X, int N, Value *R);

/* MA
	���ؼ��ƶ�ƽ��
	�÷���MA(X,M)��X��M�ռ��ƶ�ƽ�� */
Value *MA(const Value *X, int M, Value *R);

/* EMA
	����ָ���ƶ�ƽ��
	�÷���EMA(X,M)��X��M��ָ���ƶ�ƽ��
	�㷨��Y = (X*2 + Y'*(M-1)) / (M+1) */
Value *EMA(const Value *X, int M, Value *R);
		
/* SMA
	����ƽ���ƶ�ƽ��
	�÷���SMA(X,N,M)��X��N���ƶ�ƽ����MΪȨ��
	�㷨��Y = (X*M + Y'*(N-M)) / N */
Value *SMA(const Value *X, int N, int M, Value *R);

/* ------------------------------------ �ӿڿ�ʼ ------------------ */

Value *I_OPEN(void *parser, int argc, const Value **args, Value *R);
Value *I_HIGH(void *parser, int argc, const Value **args, Value *R);
Value *I_LOW(void *parser, int argc, const Value **args, Value *R);
Value *I_CLOSE(void *parser, int argc, const Value **args, Value *R);
Value *I_ADD(void *parser, int argc, const Value **args, Value *R);
Value *I_SUB(void *parser, int argc, const Value **args, Value *R);
Value *I_MUL(void *parser, int argc, const Value **args, Value *R);
Value *I_DIV(void *parser, int argc, const Value **args, Value *R);
Value *I_REF(void *parser, int argc, const Value **args, Value *R);
Value *I_MAX(void *parser, int argc, const Value **args, Value *R);
Value *I_ABS(void *parser, int argc, const Value **args, Value *R);
Value *I_HHV(void *parser, int argc, const Value **args, Value *R);
Value *I_LLV(void *parser, int argc, const Value **args, Value *R);
Value *I_MA(void *parser, int argc, const Value **args, Value *R);
Value *I_EMA(void *parser, int argc, const Value **args, Value *R);
Value *I_SMA(void *parser, int argc, const Value **args, Value *R);

/* ------------------------------------ �ӿڽ��� ------------------ */

}

#endif