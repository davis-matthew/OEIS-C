/*
 * A248340  a(n) = 10^n - 5^n.
 * https://oeis.org/A248340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - 5^n (OEIS A248340)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 10^n and 5^n using the arbitrary‑precision helper
       big_pow (seq_t base, seq_t exp).  The arguments are cast to
       seq_t so that the function works for every supported seq_t
       type.   */
    seq_t pow10 = big_pow((seq_t)10, (seq_t)n);
    seq_t pow5  = big_pow((seq_t)5,  (seq_t)n);
    return pow10 - pow5;
}
