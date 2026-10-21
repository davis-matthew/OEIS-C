/*
 * A262699  List of currency denominations such that any value x > 0 is
 *          represented in exactly x ways as a sum of distinct denominations,
 *          where a repeated value represents a bill and a coin which count as
 *          distinct denominations.
 * https://oeis.org/A262699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n=1; 2 if n=2 or n=3; 3 if n=4; 2^floor((n-1)/2) if
 *            n>4
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* behaviour below offset not required */
    if (n <= 4) {
        if (n == 1) return (seq_t)1;
        if (n == 2 || n == 3) return (seq_t)2;
        return (seq_t)3;                  /* n == 4 */
    }
    idx_t exp = (n - 1) / 2;
    return pow2(exp);
}
