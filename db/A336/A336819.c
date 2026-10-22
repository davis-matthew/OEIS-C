/*
 * A336819  Odd values of D > 0 for which the generalized Ramanujan-Nagell
 *          equation x^2 + D = 2^m has two or more solutions in the positive
 *          integers.
 * https://oeis.org/A336819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=7, a(2)=15, a(3)=23; for n>3, a(n)=2^(n+1)-1 (i.e. the odd
 *            Mersenne numbers 2^(k)-1, with the extra value D=23)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                     /* for uint64_t */
#include "../utils/pow_uint.h"           /* seq_t pow_uint(uint64_t base, idx_t exp); */

seq_t a(idx_t n)
{
    /* Handle the first three exceptional values. */
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)15;
    if (n == 3) return (seq_t)23;

    /* For n >= 4: a(n) = 2^(n+1) - 1. */
    return pow_uint(2, (idx_t)(n + 1)) - (seq_t)1;
}
