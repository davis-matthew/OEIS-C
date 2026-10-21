/*
 * A291363  The arithmetic function u(n,2,8).
 * https://oeis.org/A291363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9836, u16 n <= 9836, u32 n <= 9836, u64 n <= 9836,
 *            u128 n <= 9836, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: 
 * a(1) = 9
 * a(n) = 2                      if n is even
 * a(n) = 3                      if n is odd and divisible by 3
 * a(n) = 5                      if n is odd and divisible by 5 (and not by 3)
 * a(n) = 7                      if n is odd and divisible by 7 (and not by 3,5)
 * a(n) = 9                      otherwise (odd, no factor 3,5, or 7)
 *
 * This implements OEIS A291363: u(n,2,8), without requiring prime
 * factorisation beyond checking divisibility by 3, 5, and 7.
 */

#include <stdint.h>   /* for unavoidable integer types */

seq_t a(idx_t n)
{
    if (n == 1)          /* special case */
        return (seq_t)9;

    if ((n & 1ULL) == 0)   /* even n */
        return (seq_t)2;

    /* odd n > 1 */
    if (n % 3ULL == 0)    /* divisible by 3 */
        return (seq_t)3;
    if (n % 5ULL == 0)    /* divisible by 5 */
        return (seq_t)5;
    if (n % 7ULL == 0)    /* divisible by 7 */
        return (seq_t)7;

    /* odd n with no factor 3,5,7 */
    return (seq_t)9;
}
