/* totient: Euler's totient phi(n).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_TOTIENT_H
#define OEISC_UTILS_TOTIENT_H
#include <stdint.h>

/* Compute Euler's totient φ(m) directly by prime factorisation.
   The function works for all m ≥ 1 that fit into idx_t and the
   result also fits into seq_t (φ(m) ≤ m). */
static seq_t totient(idx_t m)
{
    if (m <= 1)
        return (seq_t)m;                 /* φ(0)=0 (unused), φ(1)=1 */

    seq_t result = (seq_t)m;
    idx_t x = m;

    /* Handle factor 2 separately. */
    if ((x & 1ULL) == 0) {
        while ((x & 1ULL) == 0)
            x >>= 1;
        result -= result / 2;
    }

    /* Trial division by odd numbers up to √x. */
    for (idx_t p = 3; p * (idx_t)p <= x; p += 2) {
        if (x % p == 0) {
            while (x % p == 0)
                x /= p;
            result -= result / p;
        }
    }

    /* If a prime > √m remains. */
    if (x > 1)
        result -= result / x;

    return result;
}

#endif
