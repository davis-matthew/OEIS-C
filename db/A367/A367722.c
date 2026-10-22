/*
 * A367722  E.g.f. satisfies A(x) = exp(x*A(-x^3)).
 * https://oeis.org/A367722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = Σ_{k=0..⌊(n-1)/3⌋} (-1)^k · (3k+1) · a(k) ·
 *            a(n-1-3k) · (n-1)! / (k!·(n-1-3k)!) where a(0)=1
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

 
static seq_t *cache = NULL;          /* stores a(0)..a(n) */
static idx_t cache_size = 0;        /* number of elements stored */

/* Extend the cache so that a(m) is available for all m <= up_to. */
static void extend_a_cache(idx_t up_to)
{
    if (cache_size > up_to)
        return;                        /* already have enough entries */

    idx_t new_size = up_to + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);
    cache = new_cache;

    /* initialize a(0) if the cache was empty */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;           /* a(0) = 1 */
        cache_size = 1;
    }

    /* compute a(n) for all n that are still missing */
    for (idx_t n = cache_size; n <= up_to; ++n) {
        idx_t m = n - 1;               /* m = n-1, so n>0 here */
        idx_t maxk = m / 3;
        seq_t sum = (seq_t)0;
        seq_t B = (seq_t)1;            /* B(0) = (n-1)! / (0!·(n-1)!) = 1 */

        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t t = m - 3 * k;       /* t = n-1-3k */

            /* term = (n-1)!/(k!·(n-1-3k)!) *
                     (-1)^k · (3k+1) · a(k) · a(t)
                 = B * (-1)^k · (3k+1) · a(k) · a(t)  (B is the binomial factor) */

            seq_t term = B;
            term = term * (seq_t)(3 * k + 1);
            if (k & 1)
                term = -term;          /* apply (-1)^k */
            term = term * cache[k];
            term = term * cache[t];
            sum += term;

            /* Update B for the next k:
               B(k+1) = B(k) * (m-3k)·(m-3k-1)·(m-3k-2) / (k+1)          */
            if (k < maxk) {
                seq_t temp = B;
                temp = temp * (seq_t)(m - 3 * k);
                temp = temp * (seq_t)(m - 3 * k - 1);
                temp = temp * (seq_t)(m - 3 * k - 2);
                B = temp / (seq_t)(k + 1);
            }
        }

        cache[n] = sum;
    }

    cache_size = new_size;
}

/* Public function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    extend_a_cache(n);
    return cache[n];
}
