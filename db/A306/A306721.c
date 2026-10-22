/*
 * A306721  a(n) = Sum_{k=0..n} binomial(k, 7*(n-k)).
 * https://oeis.org/A306721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(k, 7*(n-k)) Recurrent form: a(n) =
 *            7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5) -
 *            7*a(n-6) + a(n-7) + a(n-8) Initial values: a(0)=...=a(7)=1
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 71, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* static cache of previously computed terms */
static idx_t cache_size = 0;  /* number of terms currently in cache */

/* Compute a(n) with memoisation via the recurrence relation */
seq_t a(idx_t n)
{
    /* Ensure the cache is large enough to hold the requested term */
    if (n >= cache_size) {
        /* Determine new size (at least 8 to hold the initial 1's) */
        size_t newsize = (size_t)n + 1;
        if (newsize < 8) newsize = 8;
        /* Reallocate cache */
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure → zero */
        cache = tmp;
        /* Fill newly allocated entries */
        for (idx_t i = cache_size; i < newsize; ++i) {
            if (i < 8) {
                cache[i] = (seq_t)1;   /* first 8 terms are 1 */
            } else {
                /* compute a(i) using the recurrence */
                seq_t val  = (seq_t)7 * cache[i-1];
                val += (seq_t)35 * cache[i-3];
                val += (seq_t)21 * cache[i-5];
                val +=   cache[i-7];

                seq_t sub  = (seq_t)21 * cache[i-2];
                sub += (seq_t)35 * cache[i-4];
                sub += (seq_t)7  * cache[i-6];
                val +=   cache[i-8];

                val -= sub;   /* final value */
                cache[i] = val;
            }
        }
        cache_size = (idx_t)newsize;
    }
    return cache[n];
}
