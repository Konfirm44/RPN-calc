//trash code

int solve(handle* const top, const char op);

int is_op(const char c);

int solve(handle* const top, const char op) {	//do zaorania też
	asrt(top->head);
	double a, b;
	asrt(pop(top, &b));
	if (!(top->head)) {
		push(top, b);
		return 1;
	}
	asrt(pop(top, &a));
	switch (op) {
	case '+':
		a += b;
		break;
	case '-':
		a -= b;
		break;
	case '*':
		a *= b;
		break;
	case '/':
		a /= b;
		break;
	case '^':
		a = pow(a, b);
		break;
	default:
		return 0;
	}
	asrt(push(top, a));
	return 1;
}

int is_op(const char c) {  //do zaorania
	switch (c) {
	case '+':
	case '-':
	case '*':
	case '/':
	case '^':
		return 1;
	default:
		return 0;
	}
}

int parse_exp(char* exp, handle* const top, const args xd, FILE* f_out) {
	asrt(exp);
	if (xd.infile)
		fprintf(f_out, "exp= %s\n", exp);

	char delim[2] = " ";
	delim[0] = xd.whitespace;
	char* ptr = strtok(exp, delim);
	while (ptr) {
		double d;
		if (ptr[0] == xd.comment) {
			break;
		} else if (is_number(ptr, &d)) {
			asrt(push(top, d));
		} else if (strlen(ptr) == 1 && is_op(ptr[0])) {
			if (top->stacksize > 1) {
				asrt(solve(top, ptr[0]));
			} else {
				fprintf(stderr, "ERROR: too few operands\n");
				return 0;
			}
		} else if (strlen(ptr) == 2 && (ptr[0] == 'm' || ptr[0] == 'M')) {
			asrt(memory_operation(top, ptr[1]));
		} else {
			fprintf(stderr, "ERROR: invalid token '%s'\n", ptr);
			return 0;
		}
		ptr = strtok(NULL, delim);
	}
	if (top->stacksize > 1) {
		fprintf(stderr, "ERROR: not enough operators; %u operator(s) missing\n", (top->stacksize) - 1);
		pulverize(top);
		return 0;
	}
	return 1;
}
 

 int parse_exp_old(char* exp, handle* const top, const args xd, FILE* f_out) {
   asrt(exp);
   if (exp[0] == xd.comment) return -1;
   fprintf(f_out, "exp= %s\n", exp);
   char str[num_len_max] = "";
   for (int i = 0; i < strlen(exp); ++i) {
       //printf("%d:_%c_\tlen= %u\n", i, exp[i], strlen(exp));
       if (isdigit(exp[i]) || exp[i] == '.') {
           strncat(str, &(exp[i]), 1);
       }
       else if (exp[i] == xd.whitespace) {
           float f = strtof(str, NULL);
           asrt(push(top, f));
           str[0] = '\0';
       }
       else if (is_op(exp[i])) {
           asrt(solve(top, exp[i]));
           if (exp[i+1] == xd.whitespace)
               ++i;
       }
       else if (exp[i] == 'm' || exp[i] == 'M') {
           asrt(memory_operation(top, exp[++i]));
			if (exp[i+1] == xd.whitespace)
               ++i;
       }
       else if (exp[i] == xd.comment)
			return -1;
       else {
           fprintf(f_out, "invalid character @%d=_%c_\n", i, exp[i]);
           break;
       }
   }
   return 1;
}
 

handle *top = (handle) {.head = NULL, .memory = 0};

void print_stack(handle* const top) {  //do piachu
	stack* p = top->head;
	fprintf(stderr, "MEMORY = %0.2f\nSTACK:\t", top->memory);
	while (p) {
		fprintf(stderr, "%.2f\t", p->value);
		p = p->next;
	}
	fprintf(stderr, "END OF STACK\n");
}