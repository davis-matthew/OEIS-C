/*
 * A387476  a(n) = Sum_{k=0..floor(n/2)} 2^k * binomial(k,n-2*k)^2.
 * https://oeis.org/A387476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} 2^k * binomial(k, n-2*k)^2
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 55, u128 n <= 108,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility functions binomial and pow2 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t m = n - 2 * k;
        seq_t b = binomial(k, m);
        if (b == 0)
            continue;
        seq_t term = pow2(k);
        term *= b;
        term *= b;
        sum += term;
    }
    return sum;
}
