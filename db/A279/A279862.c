/*
 * A279862  a(n) = the smallest integer k where there are exactly n primes
 *          between 10k and 10k+100.
 * https://oeis.org/A279862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = the smallest integer k such that the interval [10k,
 *            10k+100] contains exactly n primes. For n ≥ 24 the sequence is
 *            undefined – the function returns 0
 * Range:     u8 none, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Very small sieve comfortably covers all known terms of the sequence
   (a(0)…a(22)).  The largest required k is 37027, hence *10+100 < 400 000.  */
#define SIEVE_LIMIT 400000U

static bool   *sieve_flag    = NULL; /* true → prime */
static uint32_t *prime_prefix = NULL; /* prefix prime counts */
static bool   sieve_ready   = false;

/* One‑time sieve construction.  */
static void
init_sieve(void)
{
    if (sieve_ready)
        return;

    sieve_flag    = (bool*)      malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    prime_prefix  = (uint32_t*) malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));

    /* initialise */
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i)
        sieve_flag[i] = true;
    if (SIEVE_LIMIT >= 0) sieve_flag[0] = false;
    if (SIEVE_LIMIT >= 1) sieve_flag[1] = false;

    /* Eratosthenes */
    for (size_t p = 2; p * p <= SIEVE_LIMIT; ++p)
        if (sieve_flag[p])
            for (size_t m = p * p; m <= SIEVE_LIMIT; m += p)
                sieve_flag[m] = false;

    /* prefix prime counts */
    uint32_t cnt = 0;
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) {
        if (sieve_flag[i])
            ++cnt;
        prime_prefix[i] = cnt;
    }

    sieve_ready = true;
}

/* LRU cache for the 23 known terms (0 … 22).  */
static seq_t *cache  = NULL;          /* array of seq_t size 23 */
static bool   cache_ready = false;

static const seq_t SENTINEL = (seq_t)(-1);

/* Main function required by the exercise. */
seq_t a(idx_t n)
{
    if (n > 22)        /* undefined for larger n */
        return (seq_t)0;

    init_sieve();

    if (!cache_ready) {
        /* Allocate and initialise cache */
        cache = (seq_t*)malloc(23 * sizeof(seq_t));
        for (size_t i = 0; i < 23; ++i)
            cache[i] = SENTINEL;

        /* Scan all possible k until the sieve is exhausted. */
        size_t max_k = (SIEVE_LIMIT - 100) / 10;
        for (size_t k = 0; k <= max_k; ++k) {
            size_t lo = 10 * k;
            size_t hi = lo + 100;
            size_t primes = prime_prefix[hi] -
                            (lo == 0 ? 0 : prime_prefix[lo - 1]);

            if (primes <= 22 && cache[primes] == SENTINEL)
                cache[primes] = (seq_t)k;
        }
        cache_ready = true;
    }

    /* If no k was found (should only happen for n ≥ 23), return 0. */
    return cache[n];
}
