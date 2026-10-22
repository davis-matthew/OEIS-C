/*
 * A387509  a(n) = Sum_{k=0..floor(n/3)} 2^(n-k) * binomial(n-2*k,k)^2.
 * https://oeis.org/A387509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 2^(n-k) * binomial(n-2*k,k)^2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t bin = binomial(n - 2 * k, k);
        seq_t term = pow2(n - k);
        term = term * bin * bin;
        result += term;
    }
    return result;
}
