#include <assert.h>
#include <string.h>

#include "base.h"
#include "lexer.h"
#include "parser.h"

#include "test-base.h"

static const char *FOUMULA = ""
	"DIF:EMA(CLOSE,SHORT)-EMA(CLOSE,LONG);\n"
	"DEA:EMA(DIF,MID);\n"
	"MACD:(DIF-DEA)*2;";
	
using namespace tg;

void testMACD()
{
	info("��ʼ��Ԫ����MACD\n");
	info("��ʽΪ\n%s\n", FOUMULA);
	
	void *parser = parserNew(0, testHandleError);
	
	parserParse(parser, FOUMULA, strlen(FOUMULA));
	if (!parserInterp(parser, 0)) {
		info("�������гɹ�\n");
	} else {
		warn("��������ʧ��\n");
	}
	
	parserFree(parser);
	
	info("������Ԫ����\n\n");
}
