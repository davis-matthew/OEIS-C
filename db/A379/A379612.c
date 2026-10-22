/*
 * A379612  a(n) = (n + 1)^(n - 1) + (n + 1)^(n - 2), by convention a(1) = 1.
 * https://oeis.org/A379612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^(n-1) + (n+1)^(n-2) (for n >= 0, with a(1)=1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;
    seq_t base = (seq_t)n + 1;               /* n+1 */
    seq_t exp  = (seq_t)n - 2;               /* n-2 */
    seq_t pow_value = big_pow(base, exp);    /* (n+1)^(n-2) */
    return pow_value * ((seq_t)n + 2);       /* * (n+2) gives a(n) */
}
