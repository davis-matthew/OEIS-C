/*
 * A353013  a(n) = Sum_{k=0..floor(n/2)} (n-2*k)^(n-k).
 * https://oeis.org/A353013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (n-2*k)^(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t base = n - 2 * k;      /* base value is an integer */
        idx_t exp  = n - k;          /* exponent value is an integer */
        seq_t term = seq_pow((seq_t)base, exp);
        sum += term;
    }
    return sum;
}
