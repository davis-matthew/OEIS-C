/*
 * A308938  Smallest deficient number which is the product of n consecutive
 *          primes.
 * https://oeis.org/A308938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): smallest deficient number which is a product of n consecutive primes.
   For n=0 the value is 1, for n=1 the value is 2.  For n≥2 the function
   searches the consecutive‑prime windows in order, sliding one prime at a
   time, and returns the first product that satisfies σ(product) < 2·product. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* Handle the defined offset explicitly. */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;

    /* Keep a static list of primes that is expanded as needed. */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;

    if (primes_cnt == 0) {
        primes = (uint64_t*)malloc(sizeof *primes);
        if (!primes) abort();
        primes[0] = 2;
        primes_cnt = 1;
    }

    size_t needed = (size_t)n;
    while (primes_cnt < needed) {
        uint64_t nxt = next_prime(primes[primes_cnt - 1]);
        uint64_t *tmp = (uint64_t*)realloc(primes, (primes_cnt + 1) * sizeof *primes);
        if (!tmp) abort();
        primes = tmp;
        primes[primes_cnt] = nxt;
        ++primes_cnt;
    }

    /* Compute the product and sigma for the first window. */
    size_t start = 0;
    seq_t prod  = 1;
    seq_t sigma = 1;
    for (size_t i = 0; i < needed; ++i) {
        prod  *= (seq_t)primes[start + i];
        sigma *= (seq_t)(primes[start + i] + 1);
    }

    /* Slide the window until a deficient product is found. */
    while (1) {
        if (sigma < ((seq_t)2) * prod)
            return prod;

        /* Remove the leftmost prime. */
        prod  /= (seq_t)primes[start];
        sigma /= (seq_t)(primes[start] + 1);
        ++start;

        /* Ensure the right end has another prime. */
        while (start + needed > primes_cnt) {
            uint64_t nxt = next_prime(primes[primes_cnt - 1]);
            uint64_t *tmp = (uint64_t*)realloc(primes, (primes_cnt + 1) * sizeof *primes);
            if (!tmp) abort();
            primes = tmp;
            primes[primes_cnt] = nxt;
            ++primes_cnt;
        }

        /* Add the new rightmost prime. */
        prod  *= (seq_t)primes[start + needed - 1];
        sigma *= (seq_t)(primes[start + needed - 1] + 1);
    }
}
