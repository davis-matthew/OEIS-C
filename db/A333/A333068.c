/*
 * A333068  a(1) = 1; for n > 1, a(n) = n*(n-1)/2 + ((a(n-1)-1) mod n) + 1,
 *          the a(n-1)-th term of the n-th row of the triangle of positive
 *          integers, indexed in cyclic manner.
 * https://oeis.org/A333068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = n*(n-1)/2 + ((a(n-1)-1) mod n) + 1
 *            (OEIS A333068)
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[0] = a(1), cache[1] = a(2), … */
    static seq_t *cache = NULL;
    static idx_t cached = 0;      /* number of values currently cached */

    if (n == 0) return 0; /* undefined; return 0 for convenience */

    /* Grow the cache if necessary, computing new entries in order */
    if (n > cached) {
        idx_t newsize = n;
        seq_t *newcache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!newcache) return 0;          /* allocation failure */

        /* Compute entries from cached+1 up to n */
        for (idx_t k = cached + 1; k <= n; ++k) {
            seq_t val;
            if (k == 1) {
                val = 1;                 /* a(1) = 1 */
            } else {
                seq_t prev   = newcache[k - 2];          /* a(k-1) */
                seq_t offset = (prev - 1) % (seq_t)k;    /* ((a(k-1)-1) mod k) */
                seq_t base   = (seq_t)k * (k - 1) / 2;   /* k*(k-1)/2 */
                val = base + offset + 1;
            }
            newcache[k - 1] = val;
        }

        cache = newcache;
        cached = newsize;
    }

    return cache[n - 1];
}
