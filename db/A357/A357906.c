/*
 * A357906  a(n) = log_2(A073103(n)).
 * https://oeis.org/A357906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = additive over prime powers a(2) = 0, a(4) = 1, a(8) = 2,
 *            a(2^e) = 3 for e >= 4; a(p^e) = 2 if p ≡ 1 (mod 4), 1 if p ≡ 3
 *            (mod 4). The function factors n and sums the contributions
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;          /* a(1) = 0, a(0) is irrelevant */

    uint64_t m = n;          /* mutable copy for factorisation */
    seq_t    sum = 0;        /* running sum of contributions */

    /* trial division up to sqrt(m) */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p != 0)
            continue;

        uint64_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        /* add contribution for this prime power */
        if (p == 2) {
            if (e == 1)
                sum += 0;
            else if (e == 2)
                sum += 1;
            else if (e == 3)
                sum += 2;
            else
                sum += 3;   /* e >= 4 */
        } else {
            /* odd prime: check p mod 4 */
            if (p % 4 == 1)
                sum += 2;
            else
                sum += 1;   /* p % 4 == 3 */
        }
    }

    /* if a prime factor larger than sqrt original n remains */
    if (m > 1) {
        /* m is a prime, exponent e = 1 */
        if (m == 2) {
            /* e == 1 handled above, but keep consistent */
            sum += 0;
        } else {
            if (m % 4 == 1)
                sum += 2;
            else
                sum += 1;
        }
    }

    return sum;
}
