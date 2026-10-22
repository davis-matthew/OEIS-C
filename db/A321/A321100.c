/*
 * A321100  Sequence {a(n), n>=0} satisfying the continued fraction relation:
 *          if z = [a(0) + 1; a(1) + 1, a(2) + 1, a(3) + 1, ..., a(n) + 1,
 *          ...], then 7*z = [a(0) + 9; a(1) + 11, a(2) + 11, a(3) + 11, ...,
 *          a(n) + 11, ...].
 * https://oeis.org/A321100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(9k+1) = 2 a(9k+7) = 1 a(3k) = 0 a(3k+2) = 3 – a(3k+1) a(9k+4)
 *            = 3 – a(3k+1) a(9k+5) = a(3k+1)
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* trivial cases by modulus */
    if (n % 9 == 1)          return (seq_t)2;
    if (n % 9 == 7)          return (seq_t)1;
    if (n % 3 == 0)          return (seq_t)0;

    /* n ≡ 2 (mod 3) : use a(3k+2) = 3 – a(3k+1) */
    if (n % 3 == 2)          return (seq_t)3 - a(n - 1);

    /* remaining case: n ≡ 1 (mod 3) and n % 9 == 4 */
    return (seq_t)3 - a(n / 3);
}
