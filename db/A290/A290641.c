/*
 * A290641  Multiplicative with a(p^e) = prime(p-1)^e.
 * https://oeis.org/A290641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} prime(p-1)^e where prime(k) is the k‑th prime
 *            number, and the product is over prime powers in the
 *            factorisation of n
 * Range:     u8 n <= 58, u16 n <= 2782, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies provided by the framework */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

static bool primes_initialized = false;
static idx_t *primes = NULL;
static size_t prime_count = 0;

/* Initialise the prime table once.  We need enough primes so that
   prime(p-1) exists for every prime p that can appear as a factor of n
   (primes up to ~1e5 are enough for n <= 10000). */
static void init_primes(void) {
    if (!primes_initialized) {
        /* Choose a limit that yields at least the 10 000th prime. */
        const idx_t PRIME_LIMIT = 120000;
        build_primes(PRIME_LIMIT, &primes, &prime_count);
        primes_initialized = true;
    }
}

seq_t a(idx_t n) {
    init_primes();

    if (n <= 1)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;      /* local copy for factorisation */
    seq_t result = (seq_t)1;

    /* Factor by trial division using the prime list. */
    for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            unsigned int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* prime(p-1) is the (p-1)-st prime; indices are 1‑based.
               In the 0‑based array primes[0]==2, so primes[p-2] is correct. */
            seq_t base = (seq_t)primes[p - 2];
            seq_t pow  = (seq_t)1;
            for (unsigned int j = 0; j < e; ++j)
                pow *= base;
            result *= pow;
        }
    }

    /* If a prime factor larger than sqrt(original n) remains. */
    if (m > 1) {
        uint64_t p = m;
        seq_t base = (seq_t)primes[p - 2];
        result *= base;   /* exponent is 1 */
    }

    return result;
}
