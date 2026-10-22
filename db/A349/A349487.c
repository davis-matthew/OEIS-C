/*
 * A349487  a(n) = A132739((n-5)*(n+5)).
 * https://oeis.org/A349487
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A132739((n-5)*(n+5)) = ((n-5)*(n+5)) / 5^v5((n-5)*(n+5))
 *            where v5(x) is the exponent of the prime 5 in the factorisation
 *            of x
 * Range:     u8 n <= 16, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 6 the behaviour is unspecified; return 0. */
    if (n < 6) return 0;

    seq_t m = (seq_t)(n - 5) * ((seq_t)n + 5);

    /* Remove all factors of 5 from m. */
    while (m % 5 == 0)
        m /= 5;

    return m;
}
