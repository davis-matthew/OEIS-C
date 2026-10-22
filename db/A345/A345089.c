/*
 * A345089  Averages of two consecutive odd-indexed odd primes.
 * https://oeis.org/A345089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(2*n + 1) + prime(2*n + 3))/2
 * Range:     u8 n <= 25, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

/* Cached prime array and its metadata */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_len = 0;
static idx_t prime_cache_max_needed = 0;

/* Estimate an upper bound for the n-th prime (for n >= 6). */
static size_t estimate_prime_limit(idx_t n)
{
    if (n < 6)
        return 15;
    double dn = (double)n;
    double est = dn * (log(dn) + log(log(dn))) + 3.0; /* safety margin */
    return (size_t)est;
}

/* Ensure that prime_cache contains at least need primes. */
static void ensure_primes(idx_t need)
{
    if (prime_cache_max_needed >= need)
        return; /* already have enough */

    size_t limit = estimate_prime_limit(need);
    uint64_t *primes = NULL;
    size_t cnt = 0;

    /* Grow the limit until we have enough primes. */
    while (1) {
        build_primes_upto((idx_t)limit, &primes, &cnt);
        if (cnt >= (size_t)need)
            break;
        free(primes);
        limit *= 2;   /* increase the search space */
    }

    /* Replace old cache. */
    free(prime_cache);
    prime_cache = primes;
    prime_cache_len = cnt;
    prime_cache_max_needed = need;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for offset 0, return 0 */
        return 0;

    /* Number of primes needed: prime indices 2*n+3 inclusive */
    idx_t need = 2 * n + 3;          /* 1‑based index of last required prime */

    ensure_primes(need);

    /* 0‑based indices in prime_cache */
    size_t idx1 = (size_t)(2 * n);       /* 2*n+1 → 0‑based index 2*n */
    size_t idx2 = idx1 + 2;              /* 2*n+3 → 0‑based index 2*n+2 */

    seq_t p1 = (seq_t)prime_cache[idx1];
    seq_t p2 = (seq_t)prime_cache[idx2];

    return (p1 + p2) / 2;
}
