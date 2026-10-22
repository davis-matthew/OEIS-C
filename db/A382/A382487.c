/*
 * A382487  The number of divisors of n whose largest prime factor is 3.
 * https://oeis.org/A382487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (v₂(n)+1) * v₃(n) * where v₂(n) and v₃(n) are the
 *            exponents of 2 and 3 in the prime factorisation of n. * This is
 *            equivalent to counting divisors of n of the form 2ᵃ·3ᵇ with b≥1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t e2 = 0, e3 = 0;
    /* count exponent of 2: while n is even, shift right */
    while ((n & 1) == 0) {
        n >>= 1;
        ++e2;
    }
    /* restore n for counting 3s */
    idx_t m = n;
    /* count exponent of 3 */
    while (m % 3 == 0) {
        m /= 3;
        ++e3;
    }
    /* compute (e2+1)*e3, cast to seq_t */
    return ((seq_t)(e2) + (seq_t)1) * (seq_t)e3;
}
