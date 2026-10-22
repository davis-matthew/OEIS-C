/*
 * A368332  The number of terms of A054744 that divide n.
 * https://oeis.org/A368332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A368332: number of terms of A054744 that divide n  
   Multiplicative:  for n = ∏ p^e, a(p^e) = 1 if e < p, else e − p + 2.
*/
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n <= 1) return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* handle factor 2 */
    unsigned int e = 0;
    while ((m & 1ULL) == 0) { m >>= 1; ++e; }
    if (e) {
        /* for p = 2, e ≥ p means a(2^e) = e */
        result *= (seq_t)e;
    }

    /* handle odd primes by trial division */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (e >= p) {
                /* contribution is e − p + 2 */
                result *= (seq_t)(e - p + 2);
            }
            /* if e < p the contribution is 1, so nothing to do */
        }
    }

    /* If a prime > sqrt(n) remains, its exponent is 1 (< p),
       so its contribution is 1 – nothing to multiply. */

    return result;
}
