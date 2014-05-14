#ifndef TG_INDICAOTR_INDICATORS_H
#define TG_INDICAOTR_INDICATORS_H

#include <stdint.h>

namespace tg {

struct Value {
	bool isOwnMem;
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

int isValueValid(double f); /* ֵ�Ƿ���Ч */

struct Quote {
	Value *open;
	Value *high;
	Value *low;
	Value *close;
};

/* ����ʱdataʵ����struct Quote * */
const Value *OPEN(void *data);
const Value *HIGH(void *data);
const Value *LOW(void *data);
const Value *CLOSE(void *data);

/* ����:������Value��ʱ��,�ڴ������???
 * �������
 * 	RSI
		LC:=REF(CLOSE,1);
		RSI1:SMA(MAX(CLOSE-LC,0),N1,1)/SMA(ABS(CLOSE-LC),N1,1)*100;
 * ���ʹ���ڴ������С??? 
 */

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

}

#endif