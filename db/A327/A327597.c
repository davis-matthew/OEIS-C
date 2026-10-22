/*
 * A327597  a(n) = numerator((a(n-1) + a(n-2) + 1)/a(n-1)), with a(1)=1,
 *          a(2)=2.
 * https://oeis.org/A327597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator((a(n-1) + a(n-2) + 1)/a(n-1))
 * Range:     u8 n <= 25, u16 n <= 108, u32 n <= 189, u64 n <= 852,
 *            u128 n <= 1531, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Base: a(1) = 1, a(2) = 2 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;

    seq_t a_prev = (seq_t)1;   /* a(1)   */
    seq_t a_curr = (seq_t)2;   /* a(2)   */
    seq_t a_next, numer, g;

    for (idx_t k = 3; k <= n; ++k) {
        numer = a_curr + a_prev + (seq_t)1;
        g = gcd(numer, a_curr);
        a_next = numer / g;        /* numerator after reduction */
        a_prev = a_curr;
        a_curr = a_next;
    }
    return a_curr;
}
