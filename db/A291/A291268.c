/*
 * A291268  The arithmetic function v_3(n,2).
 * https://oeis.org/A291268
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + gcd(n,6) - 2*gcd(n,3))/2
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* sequence is defined for n ≥ 2 */

    seq_t nn = (seq_t)n;          /* cast index to seq_t */
    seq_t g6 = gcd(nn, (seq_t)6); /* gcd(n,6) */
    seq_t g3 = gcd(nn, (seq_t)3); /* gcd(n,3) */

    return (nn + g6 - (g3 * 2)) / 2;
}
