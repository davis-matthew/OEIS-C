/*
 * A278930  a(n) is the least positive integer that differs (in absolute
 *          value) by an (n+1)-st power from the reverse of its binary
 *          representation.
 * https://oeis.org/A278930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = 1 + 2*(2^n + 4^(n+1)) for n > 3;
   a(1) = 2, a(2) = 36, a(3) = 100 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)36;
    if (n == 3) return (seq_t)100;

    seq_t pow2_val = pow2(n);            // 2^n
    seq_t pow4_val = pow4(n + 1);        // 4^(n+1)
    seq_t sum = pow2_val + pow4_val;     // 2^n + 4^(n+1)

    return (seq_t)1 + ((seq_t)2 * sum);  // 1 + 2*(2^n + 4^(n+1))
}
