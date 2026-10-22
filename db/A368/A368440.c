/*
 * A368440  Expansion of e.g.f. exp(x) / (3 - 2*exp(3*x)).
 * https://oeis.org/A368440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 2 * sum_{k=1..n} 3^k * binomial(n,k) * a(n-k); a(0) =
 *            1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static size_t seq_cache_size = 0;

seq_t a(idx_t n)
{
    if (seq_cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        seq_cache = tmp;
        for (idx_t i = (idx_t)seq_cache_size; i <= n; ++i) {
            if (i == 0) {
                seq_cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum  = (seq_t)0;
            seq_t binom = (seq_t)1;     /* C(i,0) */
            seq_t pow3  = (seq_t)1;     /* 3^0 */
            for (idx_t k = 1; k <= i; ++k) {
                binom = binom * (seq_t)(i - k + 1) / (seq_t)k; /* C(i,k) */
                pow3 *= (seq_t)3;                                 /* 3^k */
                sum += binom * pow3 * seq_cache[i - k];
            }
            seq_cache[i] = (seq_t)1 + (seq_t)2 * sum;
        }
        seq_cache_size = new_size;
    }
    return seq_cache[n];
}
