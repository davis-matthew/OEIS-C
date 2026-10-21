/*
 * A286888  Floor of the average gap between consecutive primes among the
 *          first n primes, for n > 1.
 * https://oeis.org/A286888
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((prime(n) – 2)/(n – 1))
 * Range:     u8 n <= 55, u16 n <= 6719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    static uint32_t *primes = NULL;
    static size_t max_n = 0;

    if ((size_t)n > max_n) {
        /* Reallocate to store the first n primes */
        if (primes)
            free(primes);
        sieve_first_n_primes(n, &primes, &max_n);
    }

    /* primes array is zero‑indexed: prime(n) is at index n-1 */
    uint32_t prime_val = primes[(size_t)(n - 1)];
    seq_t denom = (seq_t)(n - 1);

    if (prime_val <= 2)
        return 0;

    return (seq_t)((seq_t)prime_val - 2) / denom;
}
