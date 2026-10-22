/*
 * A329507  Expansion of (1 + x)*(1 + 2*x + 2*x^2 + 2*x^3 - 3*x^4) / (1 - x).
 * https://oeis.org/A329507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{min(n,5)} N_k, where N = [1,3,4,4,-1,-3]. For
 *            n >= 5, a(n) = 8
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n >= 5)
        return (seq_t)8;

    static const int64_t coeffs[6] = {1, 3, 4, 4, -1, -3};
    int64_t sum = 0;
    for (idx_t i = 0; i <= n; ++i)
        sum += coeffs[i];

    return (seq_t)sum;
}
