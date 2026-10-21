/*
 * A242333  Numbers k such that k^2 + 5 is a semiprime.
 * https://oeis.org/A242333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate k and test if k^2 + 5 is a semiprime
 * by trial division over all pre‑computed primes. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Prime sieve – once initialised, holds all primes up to LIMIT.        */
/* --------------------------------------------------------------------- */
static const idx_t PRIMES_LIMIT = 200000;           /* sufficient for n ≤ 10000 */
static bool primes_initialized = false;
static uint32_t *primes = NULL;       /* prime list */
static size_t primes_count  = 0;
static size_t primes_cap    = 0;

static void init_primes(void)
{
    if (primes_initialized) return;
    /* sieve up to PRIMES_LIMIT */
    bool *is_composite = (bool*)calloc(PRIMES_LIMIT + 1, sizeof(bool));
    if (!is_composite) exit(EXIT_FAILURE);

    primes_cap   = 1024;
    primes       = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) { free(is_composite); exit(EXIT_FAILURE); }

    for (idx_t i = 2; i <= PRIMES_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (primes_count == primes_cap) {
                primes_cap <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) { free(is_composite); free(primes); exit(EXIT_FAILURE); }
                primes = tmp;
            }
            primes[primes_count++] = (uint32_t)i;
            if (i <= PRIMES_LIMIT / i)
                for (idx_t j = i * i; j <= PRIMES_LIMIT; j += i)
                    is_composite[j] = true;
        }
    }
    free(is_composite);
    primes_initialized = true;
}

/* --------------------------------------------------------------------- */
/*  Check whether N is a semiprime (product of exactly two primes).     */
/* --------------------------------------------------------------------- */
static bool is_semiprime(seq_t N)
{
    seq_t temp = N;
    size_t factors = 0;

    for (size_t i = 0; i < primes_count; ++i) {
        uint32_t p  = primes[i];
        seq_t sp    = (seq_t)p;
        /* stop if p^2 > temp */
        if (sp * sp > temp) break;
        if (temp % sp == 0) {
            while (temp % sp == 0) {
                temp /= sp;
                ++factors;
                if (factors > 2) return false;
            }
        }
    }
    if (temp > 1) ++factors;
    return factors == 2;
}

/* --------------------------------------------------------------------- */
/*  Cached array of computed sequence terms (k values).                  */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k_to_check = 1;    /* next k to test */

/* append a new term to the cache */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
    }
    cache[cache_len++] = val;
}

/* --------------------------------------------------------------------- */
/*  Public sequence function.                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined behaviour for n=0, return 0 */

    init_primes();                 /* ensure primes at start */

    while (cache_len < n) {
        idx_t k = next_k_to_check++;
        /* N = k^2 + 5 */
        seq_t N = (seq_t)k * (seq_t)k + (seq_t)5;
        if (is_semiprime(N))
            cache_append((seq_t)k);
    }

    return cache[n - 1];
}
