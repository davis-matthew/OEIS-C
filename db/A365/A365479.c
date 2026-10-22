/*
 * A365479  The sum of unitary divisors of the smallest square divisible by n.
 * https://oeis.org/A365479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (1 + p^(e + (e mod 2)))
 * Range:     u8 n <= 14, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sum of unitary divisors of the smallest square divisible by n. */

#include <stdint.h>

/* Helper: compute base^exp using unsigned multiplication in seq_t. */
static seq_t pow_u64(uint64_t base, uint64_t exp) {
    seq_t r = 1;
    for (uint64_t i = 0; i < exp; ++i) {
        r *= (seq_t)base;
    }
    return r;
}

extern seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0; /* undefined for n=0, just return 0 */

    seq_t result = 1;
    idx_t m = n;

    /* handle factor 2 separately */
    if (m % 2 == 0) {
        idx_t e = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++e;
        }
        idx_t exp = e + (e & 1);
        seq_t term = pow_u64(2, exp);
        result *= (term + 1);
    }

    /* trial divide by odd primes up to sqrt(m) */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            idx_t exp = e + (e & 1);
            seq_t term = pow_u64(p, exp);
            result *= (term + 1);
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        /* exponent is 1 for this prime */
        seq_t term = pow_u64(m, 2);      /* m^(1 + 1) */
        result *= (term + 1);
    }

    return result;
}
