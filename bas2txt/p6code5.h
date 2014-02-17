/****************************************************/
/* bas2txt                                          */
/* p6code5.h                                        */
/* 2001.06.27. by ishioka                           */
/****************************************************/

// N66-BASIC
char p6codelist5[128][8] = {
	/* 0x80-0x8f */
	"END"	, "FOR"		, "NEXT"	, "DATA"	,
	"INPUT"	, "DIM"		, "READ"	, "LET"		,
	"GOTO"	, "RUN"		, "IF"		, "RESTORE"	,
	"GOSUB"	, "RETURN"	, "REM"		, "STOP"	,

	/* 0x90-0x9f */
	"OUT"	, "ON"		, "LPRINT"	, "DEF"		,
	"POKE"	, "PRINT"	, "CONT"	, "LIST"	,
	"LLIST"	, "CLEAR"	, "COLOR"	, "PSET"	,
	"PRESET", "LINE"	, "PAINT"	, "SCREEN"	,

	/* 0xa0-0xaf */
	"CLS"	, "LOCATE"	, "CONSOLE"	, "CLOAD"	,
	"CSAVE"	, "EXEC"	, "SOUND"	, "PLAY"	,
	"KEY"	, "LCOPY"	, "NEW"		, "RENUM"	,
	"CIRCLE", "GET"		, "PUT"		, "BLOAD"	,

	/* 0xb0-0xbf */
	"BSAVE"	, "FILES"	, "LFILES"	, "LOAD"	,
	"MERGE"	, "NAME"	, "SAVE"	, "FIELD"	,
	"LSET"	, "RSET"	, "OPEN"	, "CLOSE"	,
	"DSKO$"	, "KILL"	, "TALK"	, "MON"		,

	/* 0xc0-0xcf */
	"KANJI"	, "DELETE"	, "TAB("	, "TO"		,
	"FN"	, "SPC("	, "INKEY$"	, "THEN"	,
	"NOT"	, "STEP"	, "+"		, "-"		,
	"*"		, "/"		, "^"		, "AND"		,

	/* 0xd0-0xdf */
	"OR"	, ">"		, "="		, "<"		,
	"SGN"	, "INT"		, "ABS"		, "USR"		,
	"FRE"	, "INP"		, "LPOS"	, "POS"		,
	"SQR"	, "RND"		, "LOG"		, "EXP"		,

	/* 0xe0-0xef */
	"COS"	, "SIN"		, "TAN"		, "PEEK"	,
	"LEN"	, "HEX$"	, "STR$"	, "VAL"		,
	"ASC"	, "CHR$"	, "LEFT$"	, "RIGHT$"	,
	"MID$"	, "POINT"	, "CSRLIN"	, "STICK"	,

	/* 0xf0-0xff */
	"STRIG"	, "TIME"	, "PAD"		, "DSKI$"	,
	"LOF"	, "LOC"		, "EOF"		, "DSKF"	,
	"CVS"	, "MKS$"	, ""		, ""		,
	""		, ""		, ""		, ""		,
};
