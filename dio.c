#include "dio.h"

char * dish_readline(char ** history, int num_history, struct termios saved)
{
	char buf[256];
	memset(buf, 0, 256);
	int iter=0, miter=0, x=0, y=0;
	get_cursor(&x, &y);
	int arrow_key = 0;
	int hiter = num_history;
	char c = 1;

	while (c && c != EOF && c != '\n' && c != '\r') {
		if (iscntrl(c)) {
			if (c==3) {
				set_term(saved);
				exit(1);
			} else if (c==26) {
				struct termios tmp = get_term();
				set_term(saved);
				kill(getpid(),SIGTSTP);
				get_cursor(&x, &y);
				set_term(tmp);
			}
		}
		c = key_process(&arrow_key);
		if (arrow_key && c=='C')
			iter < miter ? ++iter : iter;
		if (arrow_key && c=='D')
			iter > 0 ? --iter : iter;
		if (arrow_key && c=='A' && num_history > 0) {
			hiter > 0 ? --hiter : hiter;
			memset(buf, 0, 256);
			int len = strlen(history[hiter]);
			memcpy(buf, history[hiter], len);
			miter = len, iter = len;
		}
		if (arrow_key && c=='B' && num_history > 0) {
			hiter=hiter < num_history-1 ? ++hiter : num_history-1;
			memset(buf, 0, 256);
			int len = strlen(history[hiter]);
			memcpy(buf, history[hiter], len);
			miter = len, iter = len;
		}
		if (!arrow_key) {
			if (miter >= 255 || iter >= 255) break;
			if (c>=0x20&&c<0x7f) {
				if (miter == iter) {
					buf[miter++, iter++] = c;
				} else {
					memmove(buf+iter+1, buf+iter, miter-iter);
					buf[miter++, iter++] = c;
				}
			} else if (c==0x7f&&iter>0) {
				if (miter==iter) {
					buf[--miter, --iter] = 0;
				} else {
					memmove(buf+iter-1, buf+iter, miter-iter);
					miter--, iter--;
				}
			}
		}

		arrow_key = 0;
		set_cursor(x, y);
		write_str("\x1b[K");
		write_buffer(buf, miter);
		set_cursor(x+iter, y);
	}

	char * str = malloc(miter+1);
	strncpy(str, buf, miter);
	str[miter] = 0;
	return str;
}

int key_process(int * arrowkey)
{
	char c = read_char();
	if (c!=0x1b) return c;
	c = read_char();
	if (c=='[') {
		*arrowkey = 1;
		return read_char();
	}
	return c;
}

char ** dish_splitline(const char * input, int *num_param)
{
	char ** args = NULL;
	int num_arg = 0;

	char * arg = dish_split(input);
	while (arg) {
		num_arg++;
		if (!args) {
			args = malloc(sizeof(char*));
		} else {
			args = realloc(args, sizeof(char*)*num_arg);
		}
		args[num_arg-1] = arg;
		arg = dish_split(NULL);
	}

	*num_param = num_arg;
	return args;
}

char * dish_split(const char * line)
{
	static const char * str = NULL;
	if (line)
		str = line;

	int squote = 0, dquote = 0, rstr = 0;
	int len = strlen(str);
	int doff = 0;
	int real_char = 0;
	int str_start = -1;
	for (int i = 0; i < len; i++) {
		if (!real_char&&(str[i]==' '||str[i]=='\t'||str[i]=='\n'))continue;
		real_char = 1;
		str_start = str_start==-1 ? i : str_start;
		if (str[i]=='\'') {
			squote = !squote;
			str_start = squote ? i+1 : str_start;
			rstr = !squote;
			doff = 1;
		}
		if (str[i]=='\"') {
			dquote = !dquote;
			str_start = dquote ? i+1 : str_start;
			rstr = !dquote;
			doff = 1;
		}

		if (!squote&&!dquote&&(str[i]==' '||str[i]=='\n'||str[i]=='\t'))
			rstr = 1;
		if ((i+1)==len)
			rstr = !rstr ? (i++, 1) : rstr;
		if (rstr) {
			int slen = i - str_start;
			char * str2 = malloc(slen+1);
			memcpy(str2, str+str_start, slen);
			str2[slen] = 0;
			str+=i+doff;
			return str2;
		}
	}

	return NULL;
}
