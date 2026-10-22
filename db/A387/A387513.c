/*
 * A387513  a(n) = Sum_{k=0..floor(n/3)} 3^(n-2*k) * binomial(n-2*k,k)^2.
 * https://oeis.org/A387513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 3^(n-2*k) * binomial(n-2*k,k)^2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 60,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t n2 = n - 2 * k;
        seq_t bin = binomial(n2, k);
        seq_t term = bin * bin;
        term = term * pow3(n2);
        sum += term;
    }
    return sum;
}
