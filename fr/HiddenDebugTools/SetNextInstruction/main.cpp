int main()
{
	int foo = 0;
	int bar = 0;

	foo = foo + 42;
	bar = bar + ((1 << 31) - 1);
	
	int* barAddress = reinterpret_cast<int*>(42);
	bar = *barAddress;
	
	return 0;
}