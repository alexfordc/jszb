#ifndef TG_INDICATOR_BASE_H
#define TG_INDICATOR_BASE_H

#ifdef WIN32
//#undef fseek
//#define fseek _fseeki64
//#undef ftell
//#define ftell _ftelli64

#define atoll _atoi64
#define strtoull _strtoi64
#define isnan _isnan

#else
/* */
#endif

namespace tg {

/* ��־�� */
enum Level {
	INFO,
	WARN,
	ERROR,
	FATAL,
	LDEBUG,
	RAWLOG, /* �����κθ�����Ϣ�� */
};

#ifdef CONFIG_LOG_DEBUG
#define debug(...) doLog(LDEBUG, __FILE__, __LINE__, __VA_ARGS__)
#else
#define debug(...) do {} while (0)
#endif
#define info(...) doLog(INFO, __FILE__, __LINE__, __VA_ARGS__)
#define warn(...) doLog(WARN, __FILE__, __LINE__, __VA_ARGS__)
#define error(...) doLog(ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define fatal(...) doLog(FATAL, __FILE__, __LINE__, __VA_ARGS__)
#define rawlog(...) doLog(RAWLOG, __FILE__, __LINE__, __VA_ARGS__)

void doLog(int level, const char *filename, int line, const char *fmt, ...);

/* ------ �ַ�����ʼ ------ */

typedef struct {
	int capacity;
	int size;
	char *data;
} String;

int stringInit(String *s, int capacity);
void stringFree(String *s);

int stringAdd(String *s, const char *str, int len);

/* ------ �ַ������� ------ */

/* ------ ���鿪ʼ ------ */

typedef struct {
	int objectSize;
	int capacity;
	int size;
	void *data;
} Array;

int arrayInit(Array *arr, int objectSize, int capacity);
void arrayFree(Array *arr);

void *arrayAdd(Array *arr);

/* ------ ������� ------ */

} // namespace tg

#endif
