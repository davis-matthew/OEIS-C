/*
 * A392643  Expansion of 1 / sqrt((1-x)^6 - 4*x^3).
 * https://oeis.org/A392643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*k,k) * binomial(n+3*k+2,
 *            n-3*k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 76,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t c1 = binomial(2 * k, k);
        seq_t c2 = binomial(n + 3 * k + 2, n - 3 * k);
        total += c1 * c2;
    }
    return total;
}
