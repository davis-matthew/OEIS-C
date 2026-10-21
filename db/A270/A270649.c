/*
 * A270649  Greatest m such that 2^m divides p^2 - q^2, where p = prime(n) and
 *          q is a prime < p.
 * https://oeis.org/A270649
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max_{q prime < prime(n)} v2(prime(n)^2 - q^2) where v2(x)
 *            is the exponent of 2 in the prime factorisation of x
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 2, old values for n < 2 are irrelevant.
       We simply return 0 for safety. */
    if (n < 2)
        return (seq_t)0;

    /* Pre‑compute a fixed list of primes once.  A limit of 200 000
       guarantees enough primes for all n up to at least 10 000
       (the 10 000th prime is 104 729). */
    static idx_t *primes = NULL;      /* array of primes: primes[0] = 2 */
    static size_t prime_count = 0;    /* number of primes stored */
    if (!primes) {
        /* build_primes_upto returns an array of type idx_t (uint64_t) */
        build_primes_upto((idx_t)200000, &primes, &prime_count);
    }

    /* Retrieve the nth prime (primes are 0‑based). */
    idx_t p = primes[n - 1];

    seq_t max_m = (seq_t)0;

    /* Examine every smaller prime q. */
    for (size_t i = 0; i < n - 1; ++i) {
        idx_t q = primes[i];
        unsigned long long diff = (unsigned long long)p * (unsigned long long)p -
                                  (unsigned long long)q * (unsigned long long)q;

        /* Count trailing zeros of diff: the exponent of 2 dividing it. */
        size_t m = 0;
        while ((diff & 1ULL) == 0) {
            diff >>= 1;
            ++m;
        }
        if (m > max_m)
            max_m = (seq_t)m;
    }

    return max_m;
}
