/*
 * A382489  The number of unitary 5-smooth divisors of n.
 * https://oeis.org/A382489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 2^([# of distinct prime factors of n that are <=5])
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n>=1 this counts the unitary 5‑smooth divisors of n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n==0 the sequence is not defined; return 0 just in case. */
    if (n == 0) return (seq_t)0;

    seq_t res = 1;
    /* factor 2 */
    if ((n & 1) == 0) res <<= 1;
    /* factor 3 */
    if (n % 3 == 0) res <<= 1;
    /* factor 5 */
    if (n % 5 == 0) res <<= 1;
    return res;
}
