/*
 * A291305  The arithmetic function v_5(n,1).
 * https://oeis.org/A291305
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - gcd(n,5)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A291305 */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2)                     /* behaviour for n < 2 is irrelevant  */
        return 0;
    seq_t nn = (seq_t)n;
    seq_t g  = gcd(nn, (seq_t)5);
    return nn - g;
}
