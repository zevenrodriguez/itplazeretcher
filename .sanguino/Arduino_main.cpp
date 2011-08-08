
extern "C" void init();
extern "C" void setup();
extern "C" void loop();

int main(void)
{
	init();
	
	setup();
    
	for (;;)
		loop();
	
	return 0;
}

