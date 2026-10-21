/*
 * A273005  Sum of coefficients in the hereditary representation of n in base
 *          10.
 * https://oeis.org/A273005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0, a(n) = n for 1 ≤ n ≤ 9, for n ≥ 10: let n = Σ dᵢ·10^eᵢ
 *            with dᵢ ∈ {1,…,9} and eᵢ ≥ 0, then a(n) = Σ (dᵢ + a(eᵢ)). (This
 *            matches the OEIS definition of A273005.)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base case: one-digit numbers */
    if (n < 10) {
        return (seq_t)n;
    }

    seq_t sum = 0;
    idx_t e = 0;             /* exponent (place value) */
    idx_t m = n;             /* remaining part of n to process */

    while (m > 0) {
        uint8_t d = m % 10;  /* current decimal digit */
        if (d != 0) {
            sum += (seq_t)d + a(e);  /* add digit and hereditary part of exponent */
        }
        m /= 10;
        e++;
    }

    return sum;
}
