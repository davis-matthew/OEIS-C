/*
 * A278801  G.f.: Sum_{k>0} x^prime(k)/(1-x^k).
 * https://oeis.org/A278801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k ≥ 1 | prime(k) ≤ n and (n - prime(k)) mod k == 0 }
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* a(0) = a(1) = 0 automatically from the loop below */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    if (n < 2)
        return (seq_t)0;

    /* Ensure we have all primes ≤ n */
    if (n > prime_limit) {
        if (primes != NULL)
            free(primes);               /* free previous array, if any */
        build_primes_upto(n, &primes, &prime_cnt);
        prime_limit = n;
    }

    seq_t count = 0;
    /* Iterate over k = 1,2,3,... corresponding to primes[0], primes[1], ... */
    for (idx_t i = 0; i < prime_cnt; ++i) {
        idx_t k        = i + 1;          /* k is the index (1‑based) */
        idx_t prime_k  = primes[i];      /* prime(k) */
        if (prime_k > n)                 /* no need to check further */
            break;
        idx_t diff = n - prime_k;
        if ((diff % k) == 0)
            ++count;
    }
    return count;
}
