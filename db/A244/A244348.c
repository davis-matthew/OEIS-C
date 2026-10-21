/*
 * A244348  Integers n such that for every integer k>0, n*10^k+1 has a divisor
 *          in the set { 11, 73, 101, 137 }.
 * https://oeis.org/A244348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑8) + 11111111 for n > 8, with a(1)=162207,
 *            a(2)=1622070, a(3)=3349554, a(4)=5109589, a(5)=6651446,
 *            a(6)=7001622, a(7)=9589051, a(8)=10958905. Indexing starts at
 *            a(1)
 * Range:     u8 none, u16 none, u32 n <= 3092, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* base cases */
    static const seq_t init[8] = {
        162207,    /* a(1) */
        1622070,   /* a(2) */
        3349554,   /* a(3) */
        5109589,   /* a(4) */
        6651446,   /* a(5) */
        7001622,   /* a(6) */
        9589051,   /* a(7) */
        10958905   /* a(8) */
    };

    if (n < 1)           /* undefined for n < 1; return 0 */
        return 0;
    if (n <= 8u)         /* direct lookup */
        return init[(size_t)(n - 1)];

    /* compute r = n mod 8, but use 8 when n is a multiple of 8 */
    idx_t r = n % 8u;
    if (r == 0u)
        r = 8u;

    /* number of times the recurrence has been applied */
    idx_t times = (n - r) / 8u;

    /* final value = init[r-1] + times * 11111111 */
    return init[(size_t)(r - 1)] + ((seq_t)times * 11111111u);
}
