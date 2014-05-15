#include "indicators.h"

#include <assert.h>
#include <float.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>

#include <limits>

#include "base.h"
#include "parser-impl.h"

namespace tg {

/* VC ����û��NAN ? */
#ifndef NAN
#define NAN (std::numeric_limits<double>::quiet_NaN())
#endif

Value *valueNew()
{
	Value *v = (Value *)malloc(sizeof(*v));
	if (!v)
		return 0;
	v->isOwnMem = false;
	v->i = 0;
	v->f = 0;
	v->values = 0;
	v->size = 0;
	v->no = 0;
	return v;
}

void valueFree(Value *v)
{
	if (v) {
		if (v->isOwnMem) {
			free(v->values);
		}
		free(v);
	}
}

bool valueExtend(Value *v, int size)
{
	assert(v && size >= 0);
	if (v->capacity < size) {
		double *mem = (double *)realloc(v->values, (sizeof(*mem) * size));
		if (!mem)
			return false;
		v->isOwnMem = true;
		v->values = mem;
		v->capacity = size;
	}
	return true;
}

double valueGet(const Value *v, int i)
{
	assert(v && v->values);
	assert(i >= 0 && i < v->size);
	return v->values[i];
}

void valueSet(Value *v, int i, double f)
{
	assert(v && v->values);
	assert(i >= 0 && i < v->size);
	v->values[i] = f;
}

int isValueValid(double f)
{
	if (isnan(f))
		return false;
	return true;
}


const Value *OPEN(void *parser)
{
	Parser *par = (Parser *)parser;
	Quote *q = (Quote *)par->userdata;
	assert(q);
	return q->open;
}

const Value *HIGH(void *parser)
{
	Parser *par = (Parser *)parser;
	Quote *q = (Quote *)par->userdata;
	assert(q);
	return q->high;
}

const Value *LOW(void *parser)
{
	Parser *par = (Parser *)parser;
	Quote *q = (Quote *)par->userdata;
	assert(q);
	return q->low;
}

const Value *CLOSE(void *parser)
{
	Parser *par = (Parser *)parser;
	Quote *q = (Quote *)par->userdata;
	assert(q);
	return q->close;
}

/* R = X + Y
 * 2���������,R�Ĵ�С��Ԫ���ٵ�����һ�� 
 * R�ͱ�ź͵�һ������������һ�� */
Value *ADD(const Value *X, const Value *Y, Value *R)
{
	int bno; /* ��ʼ��� */
	int xbno; /* X�Ŀ�ʼ��� */
	
	assert(X && Y);
	if (!X || !Y || X->size == 0 || Y->size == 0)
		return R;
	if (!R) {
		R = valueNew();
		if (!R)
			return 0;
	}
	/* ���ϴεı�ſ�ʼ
	 * X��values��size-1��ӦX->no */
	assert(R->no >= 1 && X->no >= 1 && Y->no >= 1);
	assert(X->no >= X->size);
	assert(Y->no >= Y->size);
	xbno = X->no - (X->size-1);
	bno = R->no ? R->no : (xbno);
	/* ���Y��size���ڵ���X��size����X��ʼ��
	 * ���Y��sizeС��X��size����Y�ĵ�һ��Ԫ����X�ж�Ӧ��λ�ÿ�ʼ
	 * ------------------- X
	 *      -------------- Y
	 *      -------------- R */
	if (Y->size < X->size) {
		int y0_xno = xbno + X->size - Y->size; /* Y�����еĵ�һ��Ԫ����X�еı�� */
		if (bno < y0_xno) {
			bno = y0_xno;
		}
	}
	
	int rsize = X->size > Y->size ? Y->size : X->size;
	if (!valueExtend(R, rsize)) {
		valueFree(R);
		return 0;
	}
	R->size = rsize;
	
	/* ������ĺ��濪ʼ */
	int xi = X->size - 1;
	int yi = Y->size - 1;
	int ri = R->size - 1;
	for (int eno = X->no; eno >= bno; --eno, --xi, --yi, --ri) {
		assert(xi >= 0 && xi < X->size);
		assert(yi >= 0 && yi < X->size);
		assert(ri >= 0 && ri < R->size);
		double res = valueGet(X, xi) + valueGet(Y, yi);
		valueSet(R, ri, res);
	}
	R->no = X->no;
	return R;
}

Value *SUB(const Value *X, const Value *Y, Value *R)
{
	int bno; /* ��ʼ��� */
	int xbno; /* X�Ŀ�ʼ��� */
	
	assert(X && Y);
	if (!X || !Y || X->size == 0 || Y->size == 0)
		return R;
	if (!R) {
		R = valueNew();
		if (!R)
			return 0;
	}
	/* ���ϴεı�ſ�ʼ
	 * X��values��size-1��ӦX->no */
	assert(R->no >= 1 && X->no >= 1 && Y->no >= 1);
	assert(X->no >= X->size);
	assert(Y->no >= Y->size);
	xbno = X->no - (X->size-1);
	bno = R->no ? R->no : (xbno);
	/* ���Y��size���ڵ���X��size����X��ʼ��
	 * ���Y��sizeС��X��size����Y�ĵ�һ��Ԫ����X�ж�Ӧ��λ�ÿ�ʼ
	 * ------------------- X
	 *      -------------- Y
	 *      -------------- R */
	if (Y->size < X->size) {
		int y0_xno = xbno + X->size - Y->size; /* Y�����еĵ�һ��Ԫ����X�еı�� */
		if (bno < y0_xno) {
			bno = y0_xno;
		}
	}
	
	int rsize = X->size > Y->size ? Y->size : X->size;
	if (!valueExtend(R, rsize)) {
		valueFree(R);
		return 0;
	}
	R->size = rsize;
	
	/* ������ĺ��濪ʼ */
	int xi = X->size - 1;
	int yi = Y->size - 1;
	int ri = R->size - 1;
	for (int eno = X->no; eno >= bno; --eno, --xi, --yi, --ri) {
		assert(xi >= 0 && xi < X->size);
		assert(yi >= 0 && yi < X->size);
		assert(ri >= 0 && ri < R->size);
		double res = valueGet(X, xi) - valueGet(Y, yi);
		valueSet(R, ri, res);
	}
	R->no = X->no;
	return R;
}

Value *MUL(const Value *X, const Value *Y, Value *R)
{
	int bno; /* ��ʼ��� */
	int xbno; /* X�Ŀ�ʼ��� */
	
	assert(X && Y);
	if (!X || !Y || X->size == 0 || Y->size == 0)
		return R;
	if (!R) {
		R = valueNew();
		if (!R)
			return 0;
	}
	/* ���ϴεı�ſ�ʼ
	 * X��values��size-1��ӦX->no */
	assert(R->no >= 1 && X->no >= 1 && Y->no >= 1);
	assert(X->no >= X->size);
	assert(Y->no >= Y->size);
	xbno = X->no - (X->size-1);
	bno = R->no ? R->no : (xbno);
	/* ���Y��size���ڵ���X��size����X��ʼ��
	 * ���Y��sizeС��X��size����Y�ĵ�һ��Ԫ����X�ж�Ӧ��λ�ÿ�ʼ
	 * ------------------- X
	 *      -------------- Y
	 *      -------------- R */
	if (Y->size < X->size) {
		int y0_xno = xbno + X->size - Y->size; /* Y�����еĵ�һ��Ԫ����X�еı�� */
		if (bno < y0_xno) {
			bno = y0_xno;
		}
	}
	
	int rsize = X->size > Y->size ? Y->size : X->size;
	if (!valueExtend(R, rsize)) {
		valueFree(R);
		return 0;
	}
	R->size = rsize;
	
	/* ������ĺ��濪ʼ */
	int xi = X->size - 1;
	int yi = Y->size - 1;
	int ri = R->size - 1;
	for (int eno = X->no; eno >= bno; --eno, --xi, --yi, --ri) {
		assert(xi >= 0 && xi < X->size);
		assert(yi >= 0 && yi < X->size);
		assert(ri >= 0 && ri < R->size);
		double res = valueGet(X, xi) * valueGet(Y, yi);
		valueSet(R, ri, res);
	}
	R->no = X->no;
	return R;
}

Value *DIV(const Value *X, const Value *Y, Value *R)
{
	int bno; /* ��ʼ��� */
	int xbno; /* X�Ŀ�ʼ��� */
	
	assert(X && Y);
	if (!X || !Y || X->size == 0 || Y->size == 0)
		return R;
	if (!R) {
		R = valueNew();
		if (!R)
			return 0;
	}
	/* ���ϴεı�ſ�ʼ
	 * X��values��size-1��ӦX->no */
	assert(R->no >= 1 && X->no >= 1 && Y->no >= 1);
	assert(X->no >= X->size);
	assert(Y->no >= Y->size);
	xbno = X->no - (X->size-1);
	bno = R->no ? R->no : (xbno);
	/* ���Y��size���ڵ���X��size����X��ʼ��
	 * ���Y��sizeС��X��size����Y�ĵ�һ��Ԫ����X�ж�Ӧ��λ�ÿ�ʼ
	 * ------------------- X
	 *      -------------- Y
	 *      -------------- R */
	if (Y->size < X->size) {
		int y0_xno = xbno + X->size - Y->size; /* Y�����еĵ�һ��Ԫ����X�еı�� */
		if (bno < y0_xno) {
			bno = y0_xno;
		}
	}
	
	int rsize = X->size > Y->size ? Y->size : X->size;
	if (!valueExtend(R, rsize)) {
		valueFree(R);
		return 0;
	}
	R->size = rsize;
	
	/* ������ĺ��濪ʼ */
	int xi = X->size - 1;
	int yi = Y->size - 1;
	int ri = R->size - 1;
	int rno = X->no;
	for (int kno = X->no; kno >= bno; --kno, --xi, --yi, --ri) {
		assert(xi >= 0 && xi < X->size);
		assert(yi >= 0 && yi < X->size);
		assert(ri >= 0 && ri < R->size);
		if (valueGet(Y, yi) != 0) {
			double res = valueGet(X, xi) / valueGet(Y, yi);
			valueSet(R, ri, res);
		} else {
			valueSet(R, ri, NAN);
			// ���� rno = kno;
		}
	}
	R->no = rno;
	return R;
}

/* R:=REF(X,N); 
 * �ӵ�ǰ��ǰȡN��Ԫ��
 * !!! REF���������µ��ڴ棬��X����ͬһ���ڴ� */
Value *REF(const Value *X, int N, Value *R)
{
	assert(X && N >= 0);
	if (!X || X->size == 0 || X->size < N)
		return R;
	if (!R) {
		R = valueNew();
		if (!R)
			return 0;
	}
	
	/* ----------------- X
	 *              N
	 * ------------- R */
	R->values = (double *)X->values;
	R->size = X->size - N;
	R->no = X->no - N;

	return R;
}

/* R:=MAX(X,M)
 * X > M ? X : M */
Value *MAX(const Value *X, double M, Value *R)
{
	assert(X);
	if (!X || X->size == 0)
		return R;
	if (!R) {
		R = valueNew();
		if (!R)
			return 0;
	}
	
	int rsize = X->size;
	if (!valueExtend(R, rsize)) {
		valueFree(R);
		return 0;
	}
	R->size = rsize;
	
	int xbno = X->no - (X->size-1); /* X�е�һ��Ԫ�صĿ�ʼ��� */
	int bno = R->no ? R->no : xbno; /* ��ʼ��� */
	
	/* ������ĺ��濪ʼ */
	int xi = X->size - 1;
	int ri = R->size - 1;
	for (int kno = X->no; kno >= bno; --kno, --xi, --ri) {
		assert(xi >= 0 && xi < X->size);
		assert(ri >= 0 && ri < R->size);
		double res = valueGet(X, xi);
		res = res > M ? res : M;
		valueSet(R, ri, res);
	}
	R->no = X->no;
	return R;
}

/* R:=ABS(X,M) */
Value *ABS(const Value *X, Value *R)
{
	assert(X);
	if (!X || X->size == 0)
		return R;
	if (!R) {
		R = valueNew();
		if (!R)
			return 0;
	}
	
	int rsize = X->size;
	if (!valueExtend(R, rsize)) {
		valueFree(R);
		return 0;
	}
	R->size = rsize;
	
	int xbno = X->no - (X->size-1); /* X�е�һ��Ԫ�صĿ�ʼ��� */
	int bno = R->no ? R->no : xbno; /* ��ʼ��� */
	
	/* ������ĺ��濪ʼ */
	int xi = X->size - 1;
	int ri = R->size - 1;
	for (int kno = X->no; kno >= bno; --kno, --xi, --ri) {
		assert(xi >= 0 && xi < X->size);
		assert(ri >= 0 && ri < R->size);
		double res = valueGet(X, xi);
		res = abs(res);
		valueSet(R, ri, res);
	}
	R->no = X->no;
	return R;
}

/* R:=HHV(X, N) 
 * --------------- X
 *   N  ---------- R */
Value *HHV(const Value *X, int N, Value *R)
{
	assert(X && N >= 0);
	if (!X || X->size == 0 || X->size < N)
		return R;
	if (!R) {
		R = valueNew();
		if (!R)
			return 0;
	}
	
	int rsize = X->size - N + 1;
	if (!valueExtend(R, rsize)) {
		valueFree(R);
		return 0;
	}
	R->size = rsize;
	
	int xbno = X->no - (X->size-1); /* X�е�һ��Ԫ�صĿ�ʼ��� */
	int bno = R->no ? R->no : xbno; /* ��ʼ��� */
	
	/* ������ĺ��濪ʼ */
	int xi = X->size - 1;
	int ri = R->size - 1;
	for (int kno = X->no; kno >= bno; --kno, --xi, --ri) {
		assert(xi >= 0 && xi < X->size);
		assert(ri >= 0 && ri < R->size);
		double res = valueGet(X, xi);
		for (int j = xi, count = 1; j >= 0 && count < N; --j, ++count) {
			double f = valueGet(X, j);
			if (f > res) {
				res = f;
			}
		}
		valueSet(R, ri, res);
	}
	R->no = X->no;
	return R;
}

/* R:=LLV(X, N) */
Value *LLV(const Value *X, int N, Value *R)
{
	assert(X && N >= 0);
	if (!X || X->size == 0 || X->size < N)
		return R;
	if (!R) {
		R = valueNew();
		if (!R)
			return 0;
	}
	
	int rsize = X->size - N + 1;
	if (!valueExtend(R, rsize)) {
		valueFree(R);
		return 0;
	}
	R->size = rsize;
	
	int xbno = X->no - (X->size-1); /* X�е�һ��Ԫ�صĿ�ʼ��� */
	int bno = R->no ? R->no : xbno; /* ��ʼ��� */
	
	/* ������ĺ��濪ʼ */
	int xi = X->size - 1;
	int ri = R->size - 1;
	for (int kno = X->no; kno >= bno; --kno, --xi, --ri) {
		assert(xi >= 0 && xi < X->size);
		assert(ri >= 0 && ri < R->size);
		double res = valueGet(X, xi);
		for (int j = xi, count = 1; j >= 0 && count < N; --j, ++count) {
			double f = valueGet(X, j);
			if (f < res) {
				res = f;
			}
		}
		valueSet(R, ri, res);
	}
	R->no = X->no;
	return R;
}

/* MA
	���ؼ��ƶ�ƽ��
	�÷���MA(X,M)��X��M�ռ��ƶ�ƽ�� */
Value *MA(const Value *X, int M, Value *R)
{
	assert(X && M > 0);
	if (!X || X->size == 0 || X->size < M)
		return R;
	if (!R) {
		R = valueNew();
		if (!R)
			return 0;
	}
	
	int rsize = X->size - M + 1;
	if (!valueExtend(R, rsize)) {
		valueFree(R);
		return 0;
	}
	R->size = rsize;
	
	int xbno = X->no - (X->size-1); /* X�е�һ��Ԫ�صĿ�ʼ��� */
	int bno = R->no ? R->no : xbno; /* ��ʼ��� */
	
	/* ������ĺ��濪ʼ */
	int xi = X->size - 1;
	int ri = R->size - 1;
	for (int kno = X->no; kno >= bno; --kno, --xi, --ri) {
		assert(xi >= 0 && xi < X->size);
		assert(ri >= 0 && ri < R->size);
		double res = 0;
		for (int j = xi, count = 1; j >= 0 && count < M; --j, ++count) {
			res += valueGet(X, j);
		}
		res = res / M;
		valueSet(R, ri, res);
	}
	R->no = X->no;
	return R;
}

/* EMA
	����ָ���ƶ�ƽ��
	�÷���EMA(X,M)��X��M��ָ���ƶ�ƽ��
	�㷨��Y = (X*2 + Y'*(M-1)) / (M+1) */
Value *EMA(const Value *X, int M, Value *R)
{
	assert(X && M >= 0);
	if (!X || X->size == 0)
		return R;
	if (!R) {
		R = valueNew();
		if (!R)
			return 0;
	}
	
	int rsize = X->size;
	if (!valueExtend(R, rsize)) {
		valueFree(R);
		return 0;
	}
	R->size = rsize;
	
	int xbno = X->no - (X->size-1); /* X�е�һ��Ԫ�صĿ�ʼ��� */
	int bno = R->no ? R->no : xbno; /* ��ʼ��� */
	
	/* ��ǰ������� */
	int nosize = X->no - bno + 1;
	int xi = X->size - nosize;
	int ri = R->size - nosize;
	for (int kno = bno; kno <= X->no; ++kno, ++xi, ++ri) {
		assert(xi >= 0 && xi < X->size);
		assert(ri >= 0 && ri < R->size);
		double res = valueGet(X, xi);
		if (ri > 0) { /* ��ֵ */
			double y = valueGet(R, ri-1);
			assert(isValueValid(y));
			res = (res * 2.0 + y * (M - 1)) / (M + 1);
		}
		valueSet(R, ri, res);
	}
	R->no = X->no;
	return R;
}
		
/* SMA
	����ƽ���ƶ�ƽ��
	�÷���SMA(X,N,M)��X��N���ƶ�ƽ����MΪȨ��
	�㷨��Y = (X*M + Y'*(N-M)) / N */
Value *SMA(const Value *X, int N, int M, Value *R)
{
	assert(X && M >= 0 && N > 0);
	if (!X || X->size == 0)
		return R;
	if (!R) {
		R = valueNew();
		if (!R)
			return 0;
	}
	
	int rsize = X->size;
	if (!valueExtend(R, rsize)) {
		valueFree(R);
		return 0;
	}
	R->size = rsize;
	
	int xbno = X->no - (X->size-1); /* X�е�һ��Ԫ�صĿ�ʼ��� */
	int bno = R->no ? R->no : xbno; /* ��ʼ��� */
	
	/* ��ǰ������� */
	int nosize = X->no - bno + 1;
	int xi = X->size - nosize;
	int ri = R->size - nosize;
	for (int kno = bno; kno <= X->no; ++kno, ++xi, ++ri) {
		assert(xi >= 0 && xi < X->size);
		assert(ri >= 0 && ri < R->size);
		double res = valueGet(X, xi);
		if (ri > 0) { /* ��ֵ */
			double y = valueGet(R, ri-1);
			assert(isValueValid(y));
			res = (res * M + y * (N - M)) / N;
		}
		valueSet(R, ri, res);
	}
	R->no = X->no;
	return R;
}

}
