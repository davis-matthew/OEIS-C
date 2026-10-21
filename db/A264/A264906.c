/*
 * A264906  a(n) is the denominator of the 2nd term of the power series which
 *          is the loop length in a regular n-gon. (See comment.)
 * https://oeis.org/A264906
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n^2/gcd((n-4)/gcd(n-4,4), n) for n >= 5
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 5) return (seq_t)0;        /* undefined for n < 5, not used */

    seq_t nn = (seq_t)n;               /* n as seq_t */
    seq_t n_minus4 = nn - (seq_t)4;

    /* Compute (n-4)/gcd(n-4,4) */
    seq_t g_n4_4 = gcd(n_minus4, (seq_t)4);
    seq_t num = n_minus4 / g_n4_4;     /* integer division, exact */

    /* Compute denominator gcd(num, n) */
    seq_t denom = gcd(num, nn);

    /* Final result n^2 / denom */
    seq_t result = (nn * nn) / denom;
    return result;
}
