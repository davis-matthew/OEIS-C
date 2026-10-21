/*
 * A259373  a(n) = Product_{k=0..n} p(k)^k, where p(k) is the partition
 *          function A000041.
 * https://oeis.org/A259373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} p(k)^k, p(k) = partition(k)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Allocate array for partition numbers up to n */
    seq_t *p = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (p == NULL) return 0;   /* out of memory */

    /* Base case p(0) = 1 */
    p[0] = (seq_t)1;

    /* Compute partition numbers with Euler's pentagonal recurrence */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            /* Generalized pentagonal numbers */
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            /* (+1)^(k+1) factor: + for odd k, - for even */
            if (k & 1)
                sum += p[i - pent1];
            else
                sum -= p[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            if (k & 1)
                sum += p[i - pent2];
            else
                sum -= p[i - pent2];
        }
        p[i] = sum;
    }

    /* Build the product ∏_{k=0..n} p(k)^k */
    seq_t result = (seq_t)1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t pk = p[k];
        seq_t power = big_pow(pk, (seq_t)k);   /* k ≤ n, fits in seq_t safely */
        result *= power;
    }

    free(p);
    return result;
}
