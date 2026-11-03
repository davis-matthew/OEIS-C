/*
 * A284059  The absolute values of A275966.
 * https://oeis.org/A284059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = |Re(I*Sum_{d|n}(mu(d)*I^(n/d)))|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9191 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the multiplicative property: */
/* a(p^e) = |Re(I^(p^e+1) - I^(p^(e-1)+1))| */

/* The implementation below is purely integer arithmetic and works
   for any seq_t and idx_t types.  It is O(√n) per call, easily
   under 2 s for n up to 10 000. */
#include <stdint.h>

static seq_t _value_prime_power(uint64_t p, int exp) {
    /* Compute p^e and p^(e-1) safely (they fit in uint64_t for n ≤ 10k). */
    uint64_t pe  = 1;
    uint64_t perse = 1;           /* p^(e-1) */
    for (int i = 0; i < exp; ++i) {
        pe *= p;
        if (i + 1 < exp)
            perse *= p;
    }

    /* Compute real parts of I^(p^e+1) and I^(p^(e-1)+1). */
    uint64_t A = pe + 1;          /* exponent for first term */
    uint64_t B = perse + 1;       /* exponent for second term */

    int modA = (int)(A & 3);      /* A % 4 */
    int modB = (int)(B & 3);      /* B % 4 */

    int realA = (modA == 0) ? 1 : ((modA == 2) ? -1 : 0);
    int realB = (modB == 0) ? 1 : ((modB == 2) ? -1 : 0);

    int diff = realA - realB;
    if (diff < 0) diff = -diff;
    return (seq_t)diff;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;          /* undefined offset, return 0 for safety */

    seq_t result = 1;
    uint64_t temp = (uint64_t)n;

    /* trial division up to sqrt(n) */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result *= _value_prime_power(p, exp);
        }
    }
    if (temp > 1) { /* remaining prime factor */
        result *= _value_prime_power(temp, 1);
    }

    return result;
}
