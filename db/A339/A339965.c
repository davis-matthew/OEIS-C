/*
 * A339965  a(n) = sigma(n) / gcd(sigma(n),n+1).
 * https://oeis.org/A339965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = sigma(n) / gcd(sigma(n), n+1)
 * Range:     u8 n <= 107, u16 n <= 9919, u32 n <= 9919, u64 n <= 9919,
 *            u128 n <= 9919, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include <stdint.h>

/* Compute sigma(n) for an unsigned 64‑bit integer n.
   The result is returned as a seq_t. */
static seq_t sigma_of_u64(uint64_t n)
{
    seq_t result = 1;
    uint64_t rem = n;
    unsigned int exp = 0;

    /* factor 2 */
    while ((rem & 1ULL) == 0) {
        rem >>= 1;
        exp++;
    }
    if (exp) {
        seq_t term = 1;
        seq_t pow   = 1;
        for (unsigned int k = 0; k < exp; k++) {
            pow   *= 2;
            term  += pow;
        }
        result *= term;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0) {
            exp = 0;
            while (rem % p == 0) {
                rem /= p;
                exp++;
            }
            seq_t term = 1;
            seq_t pow   = 1;
            for (unsigned int k = 0; k < exp; k++) {
                pow   *= p;
                term  += pow;
            }
            result *= term;
        }
    }

    /* remaining prime, if any */
    if (rem > 1)
        result *= (seq_t)(1 + rem);

    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset starts at 1 */
    seq_t sigma = sigma_of_u64((uint64_t)n);
    seq_t np1   = (seq_t)n + 1;
    seq_t g     = gcd(sigma, np1);
    return sigma / g;
}
