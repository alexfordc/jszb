#include <assert.h>
#include <string.h>

#include "base.h"
#include "lexer.h"
#include "parser.h"

#include "test-base.h"

static const char *FOUMULA = ""
	"SHORT:=12;\n"
	"LONG:=26;\n"
	"MID:=9;\n"
	"DIF:EMA(CLOSE,SHORT)-EMA(CLOSE,LONG);\n"
	"DEA:EMA(DIF,MID);\n"
	"MACD:(DIF-DEA)*2;";
	
using namespace tg;
namespace tg { struct Quote; }

extern tg::Quote *q;

static void *parser = 0;

void testMACDInit()
{
	info("��ʼ��Ԫ����MACD\n");
	info("��ʽΪ\n%s\n", FOUMULA);

	parser = parserNew(0, testHandleError);
	parserParse(parser, FOUMULA, strlen(FOUMULA));
}

void testMACD()
{
	if (!parserInterp(parser, q)) {
		info("�������гɹ�\n");
		double f;
		if (!parserGetIndicator(parser, "DIF", &f)) {
			info("DIF=%f\n", f);
		}
		if (!parserGetIndicator(parser, "DEA", &f)) {
			info("DEA=%f\n", f);
		}
		if (!parserGetIndicator(parser, "MACD", &f)) {
			info("MACD=%f\n", f);
		}
	} else {
		warn("��������ʧ��\n");
	}
}

void testMACDShutdown()
{
	parserFree(parser);
	info("������Ԫ����MACD\n\n");
}
