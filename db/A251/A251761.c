/*
 * A251761  10-step Fibonacci sequence starting with 0,0,0,0,0,0,1,0,0,0.
 * https://oeis.org/A251761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+10) = a(n) + a(n+1) + a(n+2) + a(n+3) + a(n+4) + a(n+5) +
 *            a(n+6) + a(n+7) + a(n+8) + a(n+9) Initial values: a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1, a(7)=0, a(8)=0,
 *            a(9)=0
 * Range:     u8 n <= 18, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* Handle the first ten terms directly to avoid allocating a cache
       for very small queries. */
    if (n < 10) {
        return (n == 6) ? (seq_t)1 : (seq_t)0;
    }

    /* Static cache for memoisation.  cap is the number of terms
       that have been computed and stored (indices 0 .. cap-1). */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;

    /* Expand the cache if we need to compute terms beyond what we already
       have stored.  oldcap keeps the previous boundary. */
    if ((size_t)n + 1 > cap) {
        size_t oldcap = cap;
        size_t newcap = (size_t)n + 1;          /* number of elements required */
        seq_t *tmp = (cache == NULL)
                 ? (seq_t *)malloc(newcap * sizeof(seq_t))
                 : (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();                      /* allocation failed */
        cache = tmp;
        cap   = newcap;

        /* Initialise the first ten terms if they are not yet set. */
        if (oldcap < 10) {
            for (size_t i = oldcap; i < 10 && i < newcap; ++i) {
                cache[i] = (i == 6) ? (seq_t)1 : (seq_t)0;
            }
        }

        /* Compute new terms starting from max(oldcap,10). */
        size_t start = (oldcap > 10) ? oldcap : 10;
        for (size_t i = start; i <= (size_t)n; ++i) {
            seq_t sum = (seq_t)0;
            for (size_t j = 0; j < 10; ++j) {
                sum += cache[i - 10 + j];
            }
            cache[i] = sum;
        }
    }

    return cache[(size_t)n];
}
