/*
 * A383752  Product of nonzero remainders n mod p, over all primes p < n.
 * https://oeis.org/A383752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all non‑zero remainders (n mod p) for primes p
 *            < n
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 47, u64 n <= 79, u128 n <= 148,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(1)=1, a(2)=1, a(3)=1, a(4)=1, ... (offset 1) */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"   /* for build_primes_upto */

seq_t a(idx_t n)
{
    /* A trivial product: start with 1 */
    seq_t prod = 1;

    /* If n <= 2, there are no primes p < n, so prod stays 1 */
    if (n <= 2) {
        return prod;
    }

    /* Build primes up to n-1 once per call */
    size_t prime_count = 0;
    uint64_t *primes = NULL;
    build_primes_upto(n - 1, &primes, &prime_count);

    /* Multiply remainder n % p for each prime; skip if remainder is zero */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        idx_t rem = n % p;
        if (rem != 0) {
            prod = prod * (seq_t)rem;
        }
    }

    /* Free memory allocated by build_primes_upto */
    free(primes);

    return prod;
}
