/*
 * A284721  Smallest odd prime that is relatively prime to 2n+1.
 * https://oeis.org/A284721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest odd prime that does not divide (2*n+1)
 * Range:     u8 n <= 540, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Generate a modest list of odd primes up to a fixed bound. */
static void init_primes(uint32_t *primes, size_t *cnt)
{
    const int LIMIT = 2000;                 /* sufficient for all tested n */
    bool is_prime[LIMIT + 1];
    for (int i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= LIMIT; j += i) is_prime[j] = false;
        }
    }
    *cnt = 0;
    for (int i = 3; i <= LIMIT; i += 2) {     /* only odd primes */
        if (is_prime[i]) primes[(*cnt)++] = (uint32_t)i;
    }
}

/* Cached list of primes. */
static uint32_t primes[256];
static size_t prime_cnt = 0;
static bool primes_inited = false;

seq_t a(idx_t n)
{
    /* Lazily initialize the prime table on first call. */
    if (!primes_inited) {
        init_primes(primes, &prime_cnt);
        primes_inited = true;
    }

    seq_t N = (seq_t)n * 2 + 1;      /* N = 2*n + 1, computed in seq_t */

    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)primes[i];         /* odd prime */
        if (N % p != 0) return p;           /* first odd prime not dividing N */
    }

    /* In practice this point is never reached for the tested range
       because a suitable prime is always found among the primes up
       to LIMIT. Fallback: return 3 (the smallest odd prime). */
    return (seq_t)3;
}
