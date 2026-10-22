/*
 * A334747  Let p be the smallest prime not dividing the squarefree part of n.
 *          Multiply n by p and divide by the product of all smaller primes.
 * https://oeis.org/A334747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * p / Π_{q < p} q * where p is the smallest prime that
 *            does NOT divide the squarefree kernel * of n (product of primes
 *            dividing n with an odd exponent). All arithmetic * is performed
 *            in seq_t. The function is independent of call order and * works
 *            for n ≥ 1
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* static cache of primes up to a fixed limit */
    static uint64_t *primes = NULL;
    static size_t primes_count = 0;
    static bool initialized = false;

    if (!initialized) {
        /* enough to cover all primes needed for n up to ~10000 */
        const idx_t prime_limit = 20000;
        build_primes_upto(prime_limit, &primes, &primes_count);
        initialized = true;
    }

    /* copy of n for factorisation */
    uint64_t n_val = (uint64_t)n;
    /* compute squarefree kernel of n (product of primes with odd exponent) */
    seq_t kernel = 1;
    for (size_t i = 0; i < primes_count && primes[i] * primes[i] <= n_val; ++i) {
        uint64_t p = primes[i];
        int parity = 0;
        while (n_val % p == 0) {
            n_val /= p;
            parity ^= 1;          /* toggle parity */
        }
        if (parity) /* odd exponent → include prime in kernel */
            kernel = kernel * (seq_t)p;
    }
    if (n_val > 1) {           /* remaining prime factor with exponent 1 */
        kernel = kernel * (seq_t)n_val;
    }

    /* find smallest prime not dividing kernel,
     * and product of all smaller primes */
    seq_t prod = 1;
    uint64_t p_small = 0;
    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes[i];
        if (kernel % p != 0) {
            p_small = p;
            break;
        }
        prod = prod * (seq_t)p;    /* multiply primes strictly less than p */
    }

    /* For safety, if we did not find p within the list (extremely unlikely) */
    if (p_small == 0) {
        /* Find next prime after the largest in the list */
        uint64_t candidate = primes[primes_count - 1] + 1;
        while (true) {
            bool is_prime = true;
            for (size_t j = 0; j < primes_count && primes[j]*primes[j] <= candidate; ++j) {
                if (candidate % primes[j] == 0) { is_prime = false; break; }
            }
            if (is_prime) {
                p_small = candidate;
                break;
            }
            ++candidate;
        }
    }

    /* compute a(n) = n * p_small / prod */
    seq_t result = ((seq_t)n) * (seq_t)p_small / prod;
    return result;
}
