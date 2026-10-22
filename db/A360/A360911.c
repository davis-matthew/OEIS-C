/*
 * A360911  Multiplicative with a(p^e) = 3*e - 2.
 * https://oeis.org/A360911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (3·e − 2) (multiplicative)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* The OEIS definition is multiplicative:   a(p^e) = 3·e − 2.
   All other a(n) are obtained from the prime factorisation. */
seq_t a(idx_t n)
{
    /* a(1) = 1 by definition; the OEIS offset is 1. */
    if (n <= 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t temp   = (uint64_t)n;          /* work in unsigned 64‑bit */
    /* handle factor 2 separately */
    uint64_t exp    = 0;
    while ((temp & 1ULL) == 0) {            /* while temp even */
        temp >>= 1;                         /* divide by 2 */
        exp++;
    }
    if (exp > 0) {
        seq_t term = (seq_t)(3ULL * exp - 2ULL);
        result *= term;
    }

    /* now temp is odd. trial divide by odd primes up to √temp. */
    for (uint64_t p = 3; p <= temp / p; p += 2) {  /* p*p <= temp, no overflow */
        if (temp % p == 0) {
            exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            seq_t term = (seq_t)(3ULL * exp - 2ULL);
            result *= term;
        }
    }

    /* If anything remains, it's a prime > √original n. */
    if (temp > 1) {
        seq_t term = (seq_t)(3ULL * 1ULL - 2ULL);   /* exponent 1 */
        result *= term;
    }

    return result;
}
