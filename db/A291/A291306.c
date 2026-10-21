/*
 * A291306  The arithmetic function v_6(n,1).
 * https://oeis.org/A291306
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - gcd(n,6)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;
    seq_t tn = (seq_t)n;
    seq_t g  = gcd(tn, (seq_t)6);
    return tn - g;
}
