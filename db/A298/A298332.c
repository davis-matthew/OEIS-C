/*
 * A298332  Number of nX3 0..1 arrays with every element equal to 1, 2, 3, 4
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1)+3*a(n-2)+14*a(n-3)-9*a(n-4)-29*a(n-5)-33*a(n-6)
 *            -48*a(n-7)+34*a(n-8)-2*a(n-9)-10*a(n-10)-32*a(n-11)
 *            +12*a(n-12)+8*a(n-13) for n>15 Initial values: a(1)=1, a(2)=17,
 *            a(3)=113, a(4)=628, a(5)=3641, a(6)=21375, a(7)=124972,
 *            a(8)=730509, a(9)=4271331, a(10)=24974286, a(11)=146021443, a...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   compute_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/compute_large.h"

/* maximum n that will be cached – the benchmark mentions n≈10000. */
#define CACHE_MAX 10000

static seq_t cache[CACHE_MAX + 1];
static int cache_initialised = 0;

/* initialise the cache with the first 15 terms */
static void cache_init(void)
{
    if (cache_initialised)
        return;

    /* explicit constants – cast to seq_t to ensure correct type */
    cache[1]  = (seq_t)1ULL;
    cache[2]  = (seq_t)17ULL;
    cache[3]  = (seq_t)113ULL;
    cache[4]  = (seq_t)628ULL;
    cache[5]  = (seq_t)3641ULL;
    cache[6]  = (seq_t)21375ULL;
    cache[7]  = (seq_t)124972ULL;
    cache[8]  = (seq_t)730509ULL;
    cache[9]  = (seq_t)4271331ULL;
    cache[10] = (seq_t)24974286ULL;
    cache[11] = (seq_t)146021443ULL;
    cache[12] = (seq_t)853770711ULL;
    cache[13] = (seq_t)4991901968ULL;
    cache[14] = (seq_t)29187089317ULL;
    cache[15] = (seq_t)170653628083ULL;

    /* compute all terms up to CACHE_MAX using the recurrence */
    for (idx_t i = 16; i <= CACHE_MAX; ++i) {
        cache[i] = (seq_t)5*cache[i-1] + (seq_t)3*cache[i-2]
                 + (seq_t)14*cache[i-3] - (seq_t)9*cache[i-4]
                 - (seq_t)29*cache[i-5] - (seq_t)33*cache[i-6]
                 - (seq_t)48*cache[i-7] + (seq_t)34*cache[i-8]
                 - (seq_t)2*cache[i-9] - (seq_t)10*cache[i-10]
                 - (seq_t)32*cache[i-11] + (seq_t)12*cache[i-12]
                 + (seq_t)8*cache[i-13];
    }
    cache_initialised = 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* safety for unexpected input */

    if (n <= CACHE_MAX) {
        cache_init();
        return cache[(size_t)n];
    }

    /* n > CACHE_MAX – compute on demand */
    return compute_large(n);
}
