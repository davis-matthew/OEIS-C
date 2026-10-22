/*
 * A343305  a(0) = ... = a(3) = 1; a(n) = a(n-4) + Sum_{k=0..n-5} a(k) *
 *          a(n-k-5).
 * https://oeis.org/A343305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-4) + Σ_{k=0}^{n-5} a(k) * a(n-k-5), with
 *            a(0)=a(1)=a(2)=a(3)=a(4)=1
 * Range:     u8 n <= 17, u16 n <= 30, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated capacity */
    static idx_t len   = 0;   /* number of entries already computed (len = cache size + 1) */

    /* initialise cache for first call */
    if (cache == NULL) {
        cap = 5;                     /* at least hold indices 0..4 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = cache[1] = cache[2] = cache[3] = cache[4] = (seq_t)1;
        len = 5;                     /* next position to be computed */
    }

    /* if requested term already computed, return it immediately */
    if (n < len)
        return cache[n];

    /* ensure we have enough storage for terms up to n */
    if ((idx_t)(len) <= n) {
        idx_t new_cap = cap;
        if (new_cap == 0) new_cap = 16;
        while (new_cap <= n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed – abort gracefully */
            return (seq_t)0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* compute any new terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t val = 0;
        if (i >= 4)
            val = cache[i - 4];
        if (i >= 5) {
            seq_t sum = 0;
            idx_t limit = i - 5;
            for (idx_t k = 0; k <= limit; ++k)
                sum += cache[k] * cache[i - k - 5];
            val += sum;
        }
        cache[i] = val;
    }
    len = n + 1;          /* all terms up to n are now computed */

    return cache[n];
}
