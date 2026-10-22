/*
 * A307727  Number of partitions of n into 3 prime powers (not including 1).
 * https://oeis.org/A307727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of partitions of n into 3 prime powers > 1
 *            (unordered, allowing repeats). The function builds a cache of
 *            all primes and prime powers up to the largest n seen and uses it
 *            to count the triples
 * Range:     u8 n <= 189, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use only standard headers that are valid for both C11 and C++17 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global static cache data --------------------------------------------- */
static idx_t   max_precompute = 0;        /* largest n for which data is ready */
static uint64_t *primes   = NULL;        /* list of primes up to max_precompute */
static size_t   primes_cnt = 0;           /* number of primes stored */
static bool   *prime_power = NULL;       /* bool array: is a number a prime power? */
static uint64_t *prime_powers = NULL;    /* sorted list of prime powers up to max_precompute */
static size_t   pp_cnt = 0;               /* number of prime powers stored */

/* Rebuild cache when a larger n is requested */
static void rebuild_cache(idx_t limit)
{
    if (limit <= max_precompute)                /* no need to rebuild */
        return;

    /* free old storage if any */
    free(primes);
    free(prime_power);
    free(prime_powers);
    primes = NULL;
    prime_power = NULL;
    prime_powers = NULL;

    /* --- build prime list up to 'limit' (simple sieve) --- */
    bool *is_composite = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    primes = (uint64_t *)calloc((size_t)(limit + 1), sizeof(uint64_t));
    primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[(size_t)i]) {
            primes[primes_cnt++] = (uint64_t)i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_composite[(size_t)j] = true;
            }
        }
    }
    free(is_composite);           /* not needed any more */

    /* --- mark prime powers in a bool array --- */
    prime_power = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        uint64_t val = p;
        while (val <= limit) {
            prime_power[(size_t)val] = true;
            if (val > limit / p)   /* avoid overflow */
                break;
            val *= p;
        }
    }

    /* --- build sorted list of all prime powers up to 'limit' --- */
    prime_powers = (uint64_t *)calloc((size_t)(limit + 1), sizeof(uint64_t));
    pp_cnt = 0;
    for (idx_t v = 2; v <= limit; ++v)
        if (prime_power[(size_t)v])
            prime_powers[pp_cnt++] = (uint64_t)v;

    max_precompute = limit;
}

/* Main sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    rebuild_cache(n);                  /* ensure cache up to n */

    seq_t N = (seq_t)n;
    seq_t total = 0;

    /* iterate with i <= j <= k where i,j,k are prime powers */
    for (size_t i = 0; i < pp_cnt; ++i) {
        seq_t a_val = (seq_t)prime_powers[i];
        if (a_val > N) break;                  /* no further i possible */
        for (size_t j = i; j < pp_cnt; ++j) {
            seq_t b_val = (seq_t)prime_powers[j];
            seq_t sum_ab = a_val + b_val;
            if (sum_ab > N) break;              /* further j only increase sum */

            seq_t rem = N - sum_ab;
            /* enforce k >= j by checking rem >= b_val */
            if (rem < b_val) continue;
            if (rem >= 2 && rem <= N && prime_power[(size_t)rem])
                ++total;
        }
    }
    return total;
}
