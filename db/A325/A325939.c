/*
 * A325939  Expansion of Sum_{k>=1} x^(2*k) / (1 + x^k).
 * https://oeis.org/A325939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = Sum_{d|n, d>1} (-1)^d
 * Range:     i8 n <= 9730, i16 n <= 9730, i32 n <= 9730, i64 n <= 9730,
 *            i128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by counting odd and even divisors from factorisation. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n = 0; safeguard */

    uint64_t m = (uint64_t)n;            /* working copy */
    seq_t D_odd = 1;                     /* product of (exp+1) for odd primes */
    uint64_t e2 = 0;                     /* exponent of 2 in n */

    /* factor out all 2's */
    while ((m & 1ULL) == 0) {
        ++e2;
        m >>= 1;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                ++exp;
                m /= p;
            }
            D_odd *= (seq_t)(exp + 1);
        }
    }

    /* remaining prime factor > 1 */
    if (m > 1)
        D_odd *= (seq_t)2;               /* exponent 1 ⇒ (1+1) = 2 */

    if (e2 == 0)                         /* n odd */
        return (seq_t)1 - D_odd;
    else                                 /* n even */
        return (seq_t)(e2 - 1) * D_odd + (seq_t)1;
}
