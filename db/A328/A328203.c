/*
 * A328203  Expansion of Sum_{k>=1} k * x^k / (1 - x^(2*k))^2.
 * https://oeis.org/A328203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*n) = 2*a(n); for odd n: a(n) = (n*d(n)+sigma(n))/2
 * Range:     u8 n <= 74, u16 n <= 5354, u32 n <= 9053, u64 n <= 9053,
 *            u128 n <= 9053, big any n
 * Helpers:   div_sigma
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/div_sigma.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0)
        return 2 * a(n >> 1);

    seq_t d, sigma_val;
    div_sigma(n, &d, &sigma_val);
    return ((seq_t)n * d + sigma_val) / 2;
}
