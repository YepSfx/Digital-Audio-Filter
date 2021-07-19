#define FORWARD 1
#define INVERSE 0

void rfft( float *x, long N, int forward );
void cfft( float *x, long NC, int forward );
void bitreverse( float *x, long N );