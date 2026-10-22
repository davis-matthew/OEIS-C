/*
 * A387507  a(n) = Sum_{k=0..floor(n/3)} 2^k * binomial(n-2*k,k)^2.
 * https://oeis.org/A387507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 2^k * binomial(n-2*k, k)^2
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 52, u128 n <= 101,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t acc = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t b = binomial(n - 2 * k, k);
        seq_t term = b * b;
        term = term * pow2(k);
        acc += term;
    }
    return acc;
}
