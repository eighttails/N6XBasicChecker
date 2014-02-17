/****************************************************/
/* bas2txt                                          */
/* p6code6.h                                        */
/* 2001.06.27. by ishioka                           */
/****************************************************/

// N66SR-BASIC
char p6codelist6[][128][8] = {
	{
		/* 0x80-0x8f */
		""			, "END"		, "FOR"		, "NEXT"	,
		"DATA"		, "INPUT"	, "DIM"		, "READ"	,
		"LET"		, "GOTO"	, "RUN"		, "IF"		,
		"RESTORE"	, "GOSUB"	, "RETURN"	, "REM"		,

		/* 0x90-0x9f */
		"STOP"		, "PRINT"	, "CLEAR"	, "LIST"	,
		"NEW"		, "ON"		, "WAIT"	, "DEF"		,
		"POKE"		, "CONT"	, "OUT"		, "LPRINT"	,
		"LLIST"		, "CONSOLE"	, "WIDTH"	, "ELSE"	,

		/* 0xa0-0xaf */
		"TRON"		, "TROFF"	, "SWAP"	, "ERASE"	,
		"MENU"		, "ERROR"	, "RESUME"	, "DELETE"	,
		"AUTO"		, "RENUM"	, "DEFSTR"	, "DEFINT"	,
		"DEFSNG"	, "DEFDBL"	, "LINE"	, "WHILE"	,

		/* 0xb0-0xbf */
		"WEND"		, "EXEC"	, "LFO"		, "PLAY"	,
		"BGM"		, "SOUND"	, "COMMON"	, "CHAIN"	,
		"CLOAD"		, "CSAVE"	, "DSKO$"	, "OPEN"	,
		"FIELD"		, "GET"		, "PUT"		, "SET"		,

		/* 0xc0-0xcf */
		"CLOSE"		, "LOAD"	, "MERGE"	, "FILES"	,
		"NAME"		, "KILL"	, "LSET"	, "RSET"	,
		"SAVE"		, "LFILES"	, "MON"		, "COLOR"	,
		"CIRCLE"	, "LCOPY"	, "CLS"		, "PSET"	,

		/* 0xd0-0xdf */
		"PRESET"	, "PAINT"	, "TERM"	, "SCREEN"	,
		"BLOAD"		, "BSAVE"	, "LOCATE"	, "BEEP"	,
		"ROLL"		, "PALET"	, "TALK"	, "KANJI"	,
		"TO"		, "THEN"	, "TAB("	, "STEP"	,

		/* 0xe0-0xef */
		"USR"		, "FN"		, "SPC("	, "NOT"		,
		"ERL"		, "ERR"		, "STRING$"	, "USING"	,
		"INSTR"		, "'"		, "VARPTR"	, "ATTR$"	,
		"DSKI$"		, "SRQ"		, "OFF"		, "INKEY$"	,

		/* 0xf0-0xff */
		">"			, "="		, "<"		, "+"		,
		"-"			, "*"		, "/"		, "^"		,
		"AND"		, "OR"		, "XOR"		, "EQV"		,
		"IMP"		, "MOD"		, "\\"		, ""		,
	}, {
		/* 0x80-0x8f */
		""			, "LEFT$"	, "RIGHT$"	, "MID$"	,
		"SGN"		, "INT"		, "ABS"		, "SQR"		,
		"RND"		, "SIN"		, "LOG"		, "EXP"		,
		"COS"		, "TAN"		, "ATN"		, "FRE"		,

		/* 0x90-0x9f */
		"INP"		, "POS"		, "LEN"		, "STR$"	,
		"VAL"		, "ASC"		, "CHR$"	, "PEEK"	,
		"SPACE$"	, "OCT$"	, "HEX$"	, "LPOS"	,
		"CINT"		, "CSNG"	, "CDBL"	, "FIX"		,

		/* 0xa0-0xaf */
		"CVI"		, "CVS"		, "CVD"		, "EOF"		,
		"LOC"		, "LOF"		, "FPOS"	, "MKI$"	,
		"MKS$"		, "MKD$"	, "STICK"	, "STRIG"	,
		"PAD"		, "GRP$"	, ""		, ""		,

		/* 0xb0-0xbf */
		""			, ""		, ""		, ""		,
		""			, ""		, ""		, ""		,
		""			, ""		, ""		, ""		,
		""			, ""		, ""		, ""		,

		/* 0xc0-0xcf */
		""			, ""		, ""		, ""		,
		""			, ""		, ""		, ""		,
		""			, ""		, ""		, ""		,
		""			, ""		, ""		, ""		,

		/* 0xd0-0xdf */
		"DSKF"		, "VIEW"	, "WINDOW"	, "POINT"	,
		"CSRLIN"	, "MAP"		, "SEARCH"	, "MOTOR"	,
		"PEN"		, "DATE$"	, "COM"		, "KEY"		,
		"TIME"		, ""		, ""		, ""		,

		/* 0xe0-0xef */
		""			, ""		, ""		, ""		,
		"CMD"		, ""		, ""		, ""		,
		""			, ""		, ""		, ""		,
		""			, ""		, ""		, ""		,

		/* 0xf0-0xff */
		""			, ""		, ""		, ""		,
		""			, ""		, ""		, ""		,
		""			, ""		, ""		, ""		,
		""			, ""		, ""		, ""		,
	},
};
