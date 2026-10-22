/*
 * A349035  G.f. A(x) satisfies: A(x) = 1 / (1 - x - x^2 * A(-2*x)).
 * https://oeis.org/A349035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 11, i64 n <= 17, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1
 * a(n) = a(n-1) + Σ_{k=0..n-2} (-2)^k * a(k) * a(n-k-2)
 */
#include <stdlib.h>
#include <stdint.h>   /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;          /* allocated size of cache */
    static idx_t  last   = 0;          /* highest index computed so far */

    /* allocate or grow cache if necessary */
    size_t need = (size_t)(n + 1);
    if (cap < need) {
        size_t newcap = (cap == 0) ? 1 : cap * 2;
        while (newcap < need) newcap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();          /* unrecoverable allocation error */
        cache = tmp;
        cap = newcap;

        /* initialize the first entry when the cache is first allocated */
        if (cache != NULL && cap > 0 && last == 0 && n >= 0) {
            cache[0] = 1;
            /* last remains 0 (value already computed) */
        }
    }

    /* compute values up to the requested index if needed */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t pow = 1;                     /* (-2)^0 */

        for (idx_t k = 0; k + 2 <= i; ++k) {
            seq_t prod = cache[k] * cache[i - 2 - k];
            sum += pow * prod;
            pow *= -2;
        }
        cache[i] = cache[i - 1] + sum;
    }
    if (n > last) last = n;

    return cache[n];
}
