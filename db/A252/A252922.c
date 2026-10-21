/*
 * A252922  a(n) = sigma(n-1) + sigma(n-2) + sigma(n-3), with a(1)=0, a(2)=1,
 *          a(3)=4.
 * https://oeis.org/A252922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = sigma(n-1) + sigma(n-2) + sigma(n-3) where sigma(k) is
 *            the sum of all positive divisors of k. Initial values: a(1)=0,
 *            a(2)=1, a(3)=4
 * Range:     u8 n <= 50, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)4;
    /* For n >= 4 compute the required sum of divisor totals. */
    return sigma_of(n-1) + sigma_of(n-2) + sigma_of(n-3);
}
