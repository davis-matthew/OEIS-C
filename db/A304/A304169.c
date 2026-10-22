/*
 * A304169  a(n) = 16*3^n + 2^(n+1) - 26 (n>=1).
 * https://oeis.org/A304169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*3^n + 2^(n+1) - 26 (n>=1)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t t3 = pow3(n);          /* 3^n */
    seq_t t2 = pow2(n + 1);      /* 2^(n+1) */
    return (seq_t)16 * t3 + t2 - (seq_t)26;
}
