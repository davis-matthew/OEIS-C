/*
 * A367939  Expansion of e.g.f. exp(exp(4*x) - 1 - 2*x).
 * https://oeis.org/A367939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -2*a(n-1) + Σ_{k=1..n} binomial(n-1,k-1)*4^k*a(n-k) OEIS
 *            A367939: Expansion of e.g.f. exp(exp(4*x) - 1 - 2*x)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < cache_len) return a_cache[n];

    size_t new_len = (size_t)(n + 1);
    a_cache = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            a_cache[0] = 1;
            continue;
        }

        seq_t sum = 0;
        seq_t pow4 = 4;
        seq_t binom = 1;

        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = binom * pow4 * a_cache[i - k];
            sum += term;

            if (k < i) {
                binom = (binom * (i - k)) / k;
            }
            pow4 *= 4;
        }

        seq_t two = 2;
        a_cache[i] = sum - two * a_cache[i - 1];
    }

    cache_len = new_len;
    return a_cache[n];
}
