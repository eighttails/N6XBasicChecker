#include "babel.cpp"

#ifdef WIN32
#define utf8_to_local(a) babel::utf8_to_sjis(a)
#else
#define utf8_to_local(a) a
#endif


int main(int argc, char *argv[])
{

    return 0;
}

