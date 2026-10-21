/*
 * A268733  a(n) = A000203(A251720(n)).
 * https://oeis.org/A268733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + p + q + p^2 + p*q + p^2*q where p = prime(n), q =
 *            prime(n+1)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* Static cache of primes, 0‑based (prime_cache[0] = 2). */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;

/* Ensure that at least idx+1 primes are cached (idx is 0‑based). */
static void ensure_prime_cache(idx_t idx)
{
    /* We need primes at positions 0 … idx, inclusive. */
    if (prime_cache_cnt >= (size_t)(idx + 1))
        return;

    /* Choose an initial numeric upper bound that guarantees at least idx+1 primes. */
    idx_t limit;
    if (idx + 1 <= 6)
        limit = 15;                       /* first six primes are < 15 */
    else
        limit = (idx + 1) * 20 + 100;      /* crude over‑estimate; sufficient for n≤10000 */

    while (prime_cache_cnt < (size_t)(idx + 1)) {
        if (prime_cache)
            free(prime_cache);
        prime_cache = NULL;
        prime_cache_cnt = 0;
        build_primes_upto(limit, &prime_cache, &prime_cache_cnt);
        if (prime_cache_cnt < (size_t)(idx + 1))
            limit *= 2;                   /* not enough; try a larger limit */
    }
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)          /* out of sequence bounds; return 0 */
        return 0;

    /* We need primes prime(n) and prime(n+1), i.e. positions n‑1 and n (0‑based). */
    ensure_prime_cache(n);

    seq_t p = (seq_t)prime_cache[n - 1];   /* prime(n)   */
    seq_t q = (seq_t)prime_cache[n];       /* prime(n+1) */

    seq_t part1 = 1 + p + p * p;           /* 1 + p + p^2          */
    seq_t part2 = 1 + q;                   /* 1 + q                */
    return part1 * part2;                  /* (1+p+p^2)*(1+q) = desired sum */
}
