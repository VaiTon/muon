int one(void);
int two(void);
int three(void);
int four(void);
int five(void);
int six(void);

int main(void)
{
	return one() + two() + three() + four() + five() + six() == 21 ? 0 : 1;
}
