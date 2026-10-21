/*
 * A280358  Number of n X 3 0..2 arrays with no element equal to more than one
 *          of its horizontal and vertical neighbors and with new values
 *          introduced in order 0 sequentially upwards.
 * https://oeis.org/A280358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) + 26*a(n-2) - 58*a(n-3) - 99*a(n-4) * -
 *            63*a(n-5) - 25*a(n-6) + 229*a(n-7) - 3*a(n-8) * - 16*a(n-9) +
 *            12*a(n-10) + 16*a(n-12) * Initial values: a(1)=4, a(2)=50,
 *            a(3)=571, a(4)=6727, a(5)=78800, * a(6)=924579, a(7)=10844773,
 *            a(8)=127214104, * a(9)=1492251709, a(10)=17504551617, *...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;       /* array of cached values, index 0 unused */
static idx_t cache_sz = 0;        /* highest index that the array can reach */

/* ensure the cache has room for indices 0..n, and initialize new slots to 0 */
static void ensure_cache(idx_t n)
{
    if (n <= cache_sz)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    for (idx_t i = cache_sz + 1; i <= n; ++i)
        cache[i] = 0;
    cache_sz = n;
}

/* compute the n-th term of A280358 */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    ensure_cache(n);

    /* initial terms, index 0 is unused */
    static const seq_t init[13] = {
        0ULL,                /* dummy for index 0 */
        4ULL, 50ULL, 571ULL, 6727ULL, 78800ULL, 924579ULL,
        10844773ULL, 127214104ULL, 1492251709ULL, 17504551617ULL,
        205333329586ULL, 2408618523801ULL
    };

    /* compute from 1 up to n, filling in any missing values */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (cache[i] != 0 || i <= 12) /* skip already stored values */
        {
            if (i <= 12)
                cache[i] = init[i];
            continue;
        }

        /* recurrence uses only previous values, which have been computed */
        seq_t v =       (seq_t)10 * cache[i-1]
                        + (seq_t)26 * cache[i-2]
                        - (seq_t)58 * cache[i-3]
                        - (seq_t)99 * cache[i-4]
                        - (seq_t)63 * cache[i-5]
                        - (seq_t)25 * cache[i-6]
                        + (seq_t)229 * cache[i-7]
                        - (seq_t)3  * cache[i-8]
                        - (seq_t)16 * cache[i-9]
                        + (seq_t)12 * cache[i-10]
                        + (seq_t)16 * cache[i-12];
        cache[i] = v;
    }

    return cache[n];
}
