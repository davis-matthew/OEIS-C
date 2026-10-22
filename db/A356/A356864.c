/*
 * A356864  a(n) is the number of primes p < n such that 2*n-p and
 *          p*(2*n-p)+2*n are also prime.
 * https://oeis.org/A356864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{p prime < n : q = 2n − p prime and r = p*q + 2n prime}|
 * Range:     u8 n <= 15, u16 n <= 1576, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

/* Computes the nth term of OEIS A356864. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Generate all primes up to 2*n (will be used for p and q). */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(2 * n, &primes, &prime_cnt);

    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = (idx_t)primes[i];
        if (p >= n)   /* we only need primes strictly less than n */
            break;

        idx_t q = 2 * n - p;
        if (!is_prime(q))
            continue;

        /* r = p*q + 2*n  < (n-1)*(2n-(n-1)) + 2n ≈ n^2 + 2n <= 1e8 for n ≤ 10000 */
        seq_t r = (seq_t)p * (seq_t)q + (seq_t)2 * n;
        if (is_prime((idx_t)r))
            ++count;
    }

    free(primes);
    return count;
}
