/*
 * A320111  Number of divisors d of n that are not of the form 4k+2.
 * https://oeis.org/A320111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n, d == 0, 1 or 3 mod 4} 1 (number of divisors of
 *            n not of the form 4k+2)
 * Range:     u8 n <= 9032, u16 n <= 9032, u32 n <= 9032, u64 n <= 9032,
 *            u128 n <= 9032, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Count powers of two in n */
    uint64_t nn = (uint64_t)n;         /* n is guaranteed <= UINT64_MAX */
    uint64_t e2 = 0;
    while ((nn & 1ULL) == 0) {         /* while nn is even */
        nn >>= 1;
        ++e2;
    }

    /* Compute divisor count of the odd part */
    seq_t odd_divcnt = 1;
    uint64_t m = nn;   /* odd part of n */

    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            odd_divcnt *= (seq_t)(cnt + 1);
        }
    }
    if (m > 1) {                 /* remaining prime factor */
        odd_divcnt *= (seq_t)2;
    }

    if (e2 == 0) {
        return odd_divcnt;       /* n is odd, standard divisor count */
    } else {
        return odd_divcnt * (seq_t)e2;  /* multiplicativity: a(2^e)=e */
    }
}
