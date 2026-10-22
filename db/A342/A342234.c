/*
 * A342234  a(n) = (27^n - 9^n)/2 - 12^n + 6^n.
 * https://oeis.org/A342234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (27^n - 9^n)/2 - 12^n + 6^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t p27 = seq_pow((seq_t)27, n);
    seq_t p9  = seq_pow((seq_t)9,  n);
    seq_t p12 = seq_pow((seq_t)12, n);
    seq_t p6  = seq_pow((seq_t)6,  n);

    /* (27^n - 9^n)/2 is integer for all n >= 0 */
    seq_t result = (p27 - p9) / 2;
    result -= p12;
    result += p6;

    return result;
}
