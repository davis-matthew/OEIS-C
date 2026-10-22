/*
 * A343781  a(n) = Sum_{k=1..floor(n/2)} sigma_k(n-k), where sigma_k(n) is the
 *          sum of the k-th powers of the divisors of n.
 * https://oeis.org/A343781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k = 1 .. floor(n/2)} sigma_k(n-k) * where sigma_k(m)
 *            is the sum of the k-th powers of all divisors of m
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

/* Compute the sum of the k-th powers of the divisors of m. */
static seq_t sigma_k(idx_t m, idx_t k)
{
    if (m == 1)
        return 1;        /* sigma_k(1) = 1 for all k */

    seq_t total = 0;
    idx_t d;
    for (d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            total += pow_seq((seq_t)d, (int)k);
            idx_t d2 = m / d;
            if (d2 != d)
                total += pow_seq((seq_t)d2, (int)k);
        }
    }
    return total;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* undefined for n = 0, guard anyway */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n / 2; ++k) {
        idx_t m = n - k;
        sum += sigma_k(m, k);
    }
    return sum;
}
