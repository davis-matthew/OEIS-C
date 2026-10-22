/*
 * A306694  a(n) is the denominator of log(A014963(n))/log(n) if n > 1 and
 *          a(1) = 1.
 * https://oeis.org/A306694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponent if n is a prime power p^exponent, otherwise 1;
 *            a(1) = 1
 * Range:     u8 n <= 9826, u16 n <= 9826, u32 n <= 9826, u64 n <= 9826,
 *            u128 n <= 9826, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 1 */
        return (seq_t)1;

    uint64_t m   = (uint64_t)n;
    seq_t   exp  = 0;

    /* Find the smallest prime factor and its exponent. */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {                 /* p is a prime factor */
            exp = 0;
            while (m % p == 0) {          /* count exponent of p */
                m /= p;
                ++exp;
            }
            break;                         /* we only need the first factor */
        }
    }

    /* If exp == 0, n is prime (no smaller divisor) or n == 1 (handled above). */
    if (exp == 0)
        return (seq_t)1;                    /* n is prime → return 1 */

    /* If after removing p^exp nothing remains, n is a prime power. */
    if (m == 1)
        return exp;                         /* prime power → exponent */

    return (seq_t)1;                         /* composite non‑prime‑power */
}
