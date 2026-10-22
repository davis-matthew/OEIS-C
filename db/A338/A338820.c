/*
 * A338820  a(n) = Sum_{i=1..n-1} (prime(i)^2 mod prime(n)).
 * https://oeis.org/A338820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n-1} (prime(i)^2 mod prime(n))
 * Range:     u8 n <= 13, u16 n <= 154, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

static uint64_t *prime_cache   = NULL; /* array of primes, 0‑indexed          */
static idx_t     prime_count   = 0;     /* number of primes stored            */
static size_t    prime_cap     = 0;     /* allocated capacity for prime_cache  */
static uint64_t  last_prime    = 0;     /* last generated prime                */

/* Ensure that prime_cache has at least n primes (indices 0..n‑1) */
static void ensure_primes(idx_t n)
{
    if (prime_count >= n)
        return;

    if (prime_cap == 0) {
        prime_cap = 16;
        prime_cache = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_cache)
            abort(); /* out of memory */
    }

    if (prime_count == 0) {
        prime_cache[prime_count++] = 2;
        last_prime = 2;
    }

    uint64_t cand = last_prime + 1;
    if (cand <= 2)
        cand = 3;
    if ((cand & 1) == 0)
        cand++;

    while (prime_count < n) {
        if (is_prime_small((seq_t)cand)) {
            if (prime_count >= prime_cap) {
                prime_cap <<= 1;
                uint64_t *tmp = (uint64_t*)realloc(prime_cache, prime_cap * sizeof(uint64_t));
                if (!tmp)
                    abort();
                prime_cache = tmp;
            }
            prime_cache[prime_count++] = cand;
            last_prime = cand;
        }
        cand += 2;          /* test next odd candidate */
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;          /* a(1) = 0 */

    ensure_primes(n);

    uint64_t prime_n = prime_cache[n - 1];
    seq_t    sum = 0;

    for (idx_t i = 1; i < n; ++i) {
        uint64_t pi  = prime_cache[i - 1];
        uint64_t val = (pi * pi) % prime_n;   /* 64‑bit arithmetic is safe here */
        sum += (seq_t)val;
    }
    return sum;
}
