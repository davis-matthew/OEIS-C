/*
 * A353017  a(n) = Sum_{k=0..floor(n/3)} (n-3*k)^(3*k).
 * https://oeis.org/A353017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (n-3*k)^(3*k)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t base = (seq_t)n - (seq_t)(3 * k);
        seq_t exp  = (seq_t)(3 * k);
        seq_t term = big_pow(base, exp);
        sum += term;
    }
    return sum;
}
