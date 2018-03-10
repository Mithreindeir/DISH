#include "dmatch.h"

int dmatch_text(const char *text, const char *match, char const***save,int*slen)
{
	int num_oper = 0;
	char operators[MAX_OPER][MAX_OPER_LEN];
	memset(operators, 0, MAX_OPER*MAX_OPER_LEN);
	dmatch_lex(match, operators, &num_oper);

	int len = dmatch_cl(text, operators, num_oper,save,slen);

	return len;
}
int dmatch_cl(const char * text,char operators[MAX_OPER][MAX_OPER_LEN],int num_oper,char const*** save,int*slen)
{
	int len = 0, asterisk = 0, kleen = 0;
	int sliter = *slen;
	const char *kstart = NULL;
	for (int i = 0; i < num_oper; i++) {
		char const** s2=*save;
		int sl=*slen;
		if (*operators[i]=='^') {
			sl++;
			if (!s2) {
				s2=malloc(sizeof(char*));
			} else {
				s2=realloc(s2,sizeof(char*)*sl);
			}
			s2[sl-1]=text+len;
			*save=s2;
			*slen=sl;
			continue;
		}
		if (!text[len]) {
			//Checks if all other operators are optional
			int optional = 1;
			while (i < num_oper) {
				int qf = *operators[i]=='?';
				int bf = *operators[i]=='{';
				int kf = *operators[i]=='$';
				int lf = *operators[i]=='^';
				if (!qf && !bf && !kf && !lf) {
					optional = 0;
					break;
				}
				i++;
			}
			if (!optional) {
				len = 0;
			}
			break;
		}

		if (kleen) {
			int ri = i;
			while (*operators[ri]!='$') ri--;
			int last = *operators[i] == '$';
			ri--;
			int l = dmatch_cl(text+len, operators+ri, 1,save,slen);
			int l2 = 0;
			if (!last) {
				l2 = dmatch_cl(text+len,operators+i, num_oper-i, save,slen);
			}
			if (l && !l2) {
				len+=l;
				if (last && !text[len]) {
					for (int j=sliter; j<(*slen);j++) {
						//(*save)[j]+=(text+len)-kstart;
					}
					continue;
				}
				i--;
				continue;
			} else if (l2) {
				for (int j = sliter; j < (*slen); j++) {
					if ((*save)[j]<(text+len))
						(*save)[j]+=(text+len)-kstart;
				}
				return len+l2;
			} else if (!l) {
				if (last) {
					continue;
				}
				len=0;
				break;
			}
		}
		if (asterisk) {
			int l = dmatch_cl(text+len, operators+i, num_oper-i,save,slen);
			if (l) {
				for (int j = sliter; j < (*slen); j++) {
					if ((*save)[j]<(text+len))
						(*save)[j]+=(text+len)-kstart;
				}
				return len+l;
			}
			len++, i--;
			continue;
		}
		int rmatch = dmatch_expr(operators[i], text[len]);
		if (rmatch==1) {
			len++;
		} else if (!rmatch && (*operators[i]!='?'&&*operators[i]!='{')) {
			len = 0;
			break;
		} else if (rmatch == -1) {
			asterisk = 1;
			sliter = *slen;
			kstart = text + len;
			if ((i+1)==num_oper)
				return len + strlen(text+len);
		} else if (rmatch == -2 && i != 0) {
			kleen = 1;
			sliter = *slen;
			kstart = text + len;
			int non_anchors = 0;
			for (int j=i+1; j < num_oper; j++)
				if (*operators[j]!='^') non_anchors++;
			if (!non_anchors)
				i--;
		}
	}
	return len;
}

void dmatch_lex(const char *match, char operators[MAX_OPER][MAX_OPER_LEN], int *num_oper)
{

	int num_o = 0;
	char *cur = NULL;
	int clen = 0;
	while (*match) {
		clen = 1;
		if (*match=='^')
			clen = 1;
		else if (*match=='$')
			clen = 1;
		else if (*match=='*')
			clen = 1;
		else if (*match=='?')
			clen = 1;
		else if (*match == '[') {
			while (match[clen] && match[clen] != ']') clen++;
			clen++;
		}
		else if (*match == '{') {
			while (match[clen] && match[clen] != '}') clen++;
			clen++;
		}
		num_o++;
		memcpy(operators[num_o-1], match, clen);
		operators[num_o-1][clen] = 0;
		match += clen;
	}

	*num_oper = num_o;
}

int dmatch_expr(char *oper, char c)
{
	if (*oper=='$') return -2;
	if (*oper=='*') return -1;
	if (*oper=='?') return 1;
	if (*oper==c) return 1;

	int not = 0;
	if (*oper=='[') {
		oper++;
		while (*oper && *oper != ']') {
			if (*oper=='!') {
				not = !not;
				oper++;
				continue;
			}
			int range = *(oper+1)=='-';
			if (!range && *oper==c) return !not;
			else if (range) {
				char mrange = *(oper+2);
				if (c>=*oper&&c<=mrange)
					return !not;
				oper++;
			}
			oper++;
		}
		return not;
	}
	not = 0;
	if (*oper=='{') {
		oper++;
		while (*oper && *oper != '}') {
			if (*oper=='!') {
				not = !not;
				oper++;
				continue;
			}
			int range = *(oper+1)=='-';
			if (!range && *oper==c) return !not;
			else if (range) {
				char mrange = *(oper+2);
				if (c>=*oper&&c<=mrange)
					return !not;
				oper++;
			}
			oper++;
		}
		return not;
	}
	return 0;
}
