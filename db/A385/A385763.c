/*
 * A385763  G.f. A(x) satisfies A(x) = 1/(1 - x*A(x) - x^4*A'''(x)).
 * https://oeis.org/A385763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = ∑_{k=0}^{n-1} (1 + 2*k - 3*k^2 + k^3) * a(k) *
 *            a(n-1-k) for n ≥ 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holding all computed terms           */
    static seq_t *cache = NULL;     /* cache[0 .. len-1] contain a(0) … a(len-1) */
    static idx_t len   = 0;         /* number of computed terms (next free slot) */

    /* initialise cache with a(0) = 1 */
    if (len == 0) {
        len  = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * len);
        cache[0] = (seq_t)1;
    }

    /* grow cache if necessary */
    if (n + 1 > len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) { /* out of memory: abort the program */
            abort();
        }
        cache = tmp;
        /* compute new terms from old_len up to n (inclusive) */
        for (idx_t i = len; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < i; ++k) {
                seq_t a_k = cache[k];
                seq_t a_j = cache[i - 1 - k];
                seq_t sk  = (seq_t)k;
                /* f(k) = 1 + 2*sk - 3*sk^2 + sk^3 */
                seq_t term = (seq_t)1 + sk * (seq_t)2
                                - sk * sk * (seq_t)3
                                + sk * sk * sk;
                sum += term * a_k * a_j;
            }
            cache[i] = sum;
        }
        len = new_len;
    }

    return cache[n];
}
