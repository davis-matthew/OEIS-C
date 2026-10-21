/*
 * A297298  Number of nX7 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 2 neighboring 1s.
 * https://oeis.org/A297298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) -6*a(n-2) -26*a(n-3) +18*a(n-4) +37*a(n-5)
 *            -25*a(n-7) -9*a(n-8) -6*a(n-9) -2*a(n-10) +6*a(n-11) +5*a(n-12)
 *            for n > 14, with a(1)=1, a(2)=9, a(3)=32, a(4)=130, a(5)=600,
 *            a(6)=2816, a(7)=13504, a(8)=65521, a(9)=319835, a(10)=1566612,
 *            a(11)=7687400, a(12)=37759082, a(13)=185561978, a...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#ifndef MAX_N
#define MAX_N 10000
#endif

static seq_t cache[MAX_N + 1];
static idx_t cache_filled = 0;

/* initialise the cache with the first 14 terms */
static void init_cache(void)
{
    if (cache_filled) return;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)9;
    cache[3] = (seq_t)32;
    cache[4] = (seq_t)130;
    cache[5] = (seq_t)600;
    cache[6] = (seq_t)2816;
    cache[7] = (seq_t)13504;
    cache[8] = (seq_t)65521;
    cache[9] = (seq_t)319835;
    cache[10] = (seq_t)1566612;
    cache[11] = (seq_t)7687400;
    cache[12] = (seq_t)37759082;
    cache[13] = (seq_t)185561978;
    cache[14] = (seq_t)912173296;
    cache_filled = 14;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined, but return 0 */

    if (n > MAX_N) {
        /* if n is out of bounds (rare) dynamically compute without caching */
        init_cache();
        if (n <= cache_filled) return cache[(idx_t)n];

        seq_t val = 0;
        for (idx_t i = cache_filled + 1; i <= n; ++i) {
            seq_t pos = 0, neg = 0;
            pos += (seq_t)7 * cache[i-1];
            pos += (seq_t)18 * cache[i-4];
            pos += (seq_t)37 * cache[i-5];
            pos += (seq_t)6 * cache[i-11];
            pos += (seq_t)5 * cache[i-12];

            neg += (seq_t)6 * cache[i-2];
            neg += (seq_t)26 * cache[i-3];
            neg += (seq_t)25 * cache[i-7];
            neg += (seq_t)9 * cache[i-8];
            neg += (seq_t)6 * cache[i-9];
            neg += (seq_t)2 * cache[i-10];

            val = pos - neg;
        }
        return val;
    }

    /* n <= MAX_N, use memoized cache */
    init_cache();

    if (n <= cache_filled) return cache[(idx_t)n];

    for (idx_t i = cache_filled + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        pos += (seq_t)7 * cache[i-1];
        pos += (seq_t)18 * cache[i-4];
        pos += (seq_t)37 * cache[i-5];
        pos += (seq_t)6 * cache[i-11];
        pos += (seq_t)5 * cache[i-12];

        neg += (seq_t)6 * cache[i-2];
        neg += (seq_t)26 * cache[i-3];
        neg += (seq_t)25 * cache[i-7];
        neg += (seq_t)9 * cache[i-8];
        neg += (seq_t)6 * cache[i-9];
        neg += (seq_t)2 * cache[i-10];

        cache[i] = pos - neg;
    }

    cache_filled = n;
    return cache[(idx_t)n];
}
