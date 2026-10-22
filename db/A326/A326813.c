/*
 * A326813  Dirichlet g.f.: zeta(2*s) / (1 - 2^(-s)).
 * https://oeis.org/A326813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 0 if n has an odd prime factor with an odd exponent;
 *            otherwise a(n) = floor(v₂(n)/2) + 1, where v₂(n) is the exponent
 *            of 2 in n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is multiplicative.

   Implementation factors n into 2's and odd primes.
   If any odd prime exponent is odd, the result is zero.
   Otherwise the result is (floor(e₂/2)+1). */

#include <stdint.h>

/* Return a(n) for the OEIS sequence A326813 */
seq_t a(idx_t n) {
    if (n == 0) {
        /* Sequence defined only for n ≥ 1; callers must respect that. */
        return (seq_t)0;
    }

    idx_t tmp = n;
    unsigned v2 = 0;
    /* Count the exponent of 2 */
    while ((tmp & 1ULL) == 0ULL) {
        v2++;
        tmp >>= 1;
    }

    /* Base result from the 2‑part */
    seq_t result = (seq_t)(v2 / 2) + (seq_t)1;

    /* If the remaining part is 1, we are done */
    if (tmp == 1ULL) {
        return result;
    }

    /* Factor the odd part; abort if any odd exponent is odd */
    idx_t m = tmp;
    while (m > 1ULL) {
        idx_t d = 3ULL;
        unsigned exp = 0;

        /* Find the smallest odd divisor of m */
        for (d = 3ULL; d * d <= m; d += 2ULL) {
            if (m % d == 0ULL) {
                while (m % d == 0ULL) {
                    m /= d;
                    exp++;
                }
                break; /* divisor found */
            }
        }

        /* If no divisor was found, m itself is prime */
        if (d * d > m) {
            if (m > 1ULL) {          /* m is a prime > 1 */
                exp = 1;
                m = 1;
            }
        }

        /* Odd exponent of an odd prime makes the whole result zero */
        if (exp & 1U) {
            return (seq_t)0;
        }
        /* Even exponent contributes nothing (factor 1) */
    }

    return result;
}
