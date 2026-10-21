/*
 * A277543  a(n) = n/5^m mod 5, where 5^m is the greatest power of 5 that
 *          divides n.
 * https://oeis.org/A277543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n / 5^m) mod 5, where 5^m is the greatest power of 5
 *            dividing n (right‑most non‑zero digit in the base‑5
 *            representation of n)
 * Range:     u8 n <= 774, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* not part of the defined orbit, but safe */

    seq_t x = (seq_t)n;
    while ((x % 5) == 0)          /* strip all factors of 5 */
        x = x / 5;

    return x % 5;                 /* now x is not divisible by 5 */
}
