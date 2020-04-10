#ifndef MYUART_H_
#define MYUART_H_


#define BuadRate 9600

extern unsigned int FreqLevel;
extern int uartsetup;

#define DEBUG 1

// Init serial
void uartinit();
// Serial printf
void _DBGUART(char* format,...);
// dummy function
void dummyprint(char* format,...);

// Serial printf for debugging
#ifdef DEBUG
static void (*dprint2uart)(char* format,...) = _DBGUART;
#else
static void (*dprint2uart)(char* format,...) = dummyprint;
#endif

//void dprint2uart(char* format,...);
//Put a string to serial
void print2uartlength(char* str,int length);
//Convert integer to a string
char *convert(unsigned int num, int base);
//Convert long integer to a string
char *convertl(unsigned long num, int base);

unsigned long getFrequency(int level);
void setFrequency(int level);


#endif /* MYUART_H_ */
