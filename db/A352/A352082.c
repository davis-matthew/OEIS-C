/*
 * A352082  a(n) = Sum_{k=0..floor(n/2)} (n-2*k)^n.
 * https://oeis.org/A352082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (n-2*k)^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n >> 1;          /* floor(n/2) */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t base = (seq_t)n - (seq_t)(2 * k);
        seq_t term = big_pow(base, (seq_t)n);
        sum += term;
    }
    return sum;
}
