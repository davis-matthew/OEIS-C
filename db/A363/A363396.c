/*
 * A363396  a(n) = Sum_{k=0..n} 2^(n - k) * Sum_{j=0..k} binomial(k, j) * (2*j
 *          + 1)^n. Row sums of A363398.
 * https://oeis.org/A363396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k) * Sum_{j=0..k} binomial(k, j) * (2*j
 *            + 1)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"   /* declares seq_t binomial(idx_t, idx_t) */
#include "../utils/pow_uint.h"    /* declares seq_t pow_uint(uint64_t, idx_t) */

seq_t a(idx_t n)
{
    /* precompute powers (2*j + 1)^n for j = 0..n */
    seq_t *pow_odd = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!pow_odd) return 0;   /* out of memory: return 0 (should not happen) */
    for (idx_t j = 0; j <= n; ++j)
        pow_odd[j] = pow_uint(2 * j + 1, n);

    /* precompute powers 2^d for d = 0..n */
    seq_t *pow_two = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!pow_two) { free(pow_odd); return 0; }
    pow_two[0] = 1;
    for (idx_t d = 1; d <= n; ++d)
        pow_two[d] = pow_two[d - 1] * 2;

    seq_t result = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t inner_sum = 0;
        seq_t C = 1;                /* binomial(k, 0) */
        for (idx_t j = 0; j <= k; ++j)
        {
            inner_sum += C * pow_odd[j];
            if (j < k)
                C = C * (k - j) / (j + 1);  /* update to binomial(k, j+1) */
        }
        result += pow_two[n - k] * inner_sum;
    }

    free(pow_odd);
    free(pow_two);
    return result;
}
