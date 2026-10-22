/*
 * A352467  a(0) = 1; a(n) = Sum_{k=1..n} binomial(2*n,2*k)^2 * a(n-k).
 * https://oeis.org/A352467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} C(2*n,2*k)^2 * a(n-k), a(0) = 1 where C(n,k)
 *            is the binomial coefficient. The implementation uses dynamic
 *            programming with memoisation. The binomial coefficients
 *            C(2*n,2*k) are produced incrementally using the recurrence
 *            w_{k+1} = w_k * (2n-2k+2)*(2n-2k+1) / (2k*(2k-1)) which...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>   /* for NULL, size_t */

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of entries allocated */

    /* lazy allocation for n == 0 */
    if (n == 0) {
        if (cache_cap < 1) {
            cache_cap = 1;
            cache = (seq_t *)malloc(sizeof(seq_t));
            cache[0] = (seq_t)1;
        }
        return cache[0];
    }

    /* grow cache if necessary */
    if (cache_cap <= (size_t)n) {
        size_t old_cap = cache_cap;
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));

        /* initialise base case if this is the first allocation */
        if (old_cap == 0) {
            cache[0] = (seq_t)1;
        }

        /* compute terms from old_cap (exclusive) up to n (inclusive) */
        idx_t start = old_cap;
        if (start == 0) {
            start = 1;   /* skip re‑computing a(0) */
        }
        for (idx_t i = start; i <= n; ++i) {
            seq_t sum = (seq_t)0;         /* a(i) */
            seq_t w   = (seq_t)1;         /* C(2*i, 0) */

            for (idx_t k = 1; k <= i; ++k) {
                /* compute ratio to obtain C(2*i, 2*k) from w = C(2*i,2*(k-1)) */
                seq_t num1 = (seq_t)(2 * i - 2 * k + 2);
                seq_t num2 = (seq_t)(2 * i - 2 * k + 1);
                seq_t den1 = (seq_t)(2 * k);
                seq_t den2 = (seq_t)(2 * k - 1);

                w = w * num1;
                w = w * num2;
                w = w / (den1 * den2);   /* exact division */

                seq_t wsq = w * w;          /* C(2*i,2*k)^2 */
                sum += wsq * cache[i - k];
            }
            cache[i] = sum;
        }
    }

    return cache[n];
}
