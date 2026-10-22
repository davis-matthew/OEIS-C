/*
 * A332656  Number of decompositions of 2n into unordered sums of two odd
 *          primes, including at least one twin prime.
 * https://oeis.org/A332656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered decompositions of 2n * into two
 *            primes, of which at least one is a twin prime
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* External types (supplied by the build system) */
/* #define SEQ_T_DEFINED   // used internally to exclude redefinition */
/* typedef ... seq_t;   */
/* typedef ... idx_t;   */

static idx_t  cache_size   = 0;   /* allocated size of cache array */
static seq_t *cache        = NULL;/* cache[n] holds a(n) for n>=1 */
static idx_t  max_n_cached = 0;   /* largest n that has been computed */

/* Sieve data */
static idx_t  sieve_limit = 0;   /* maximum value for which the sieve is built   */
static bool   *is_prime   = NULL;/* is_prime[i] == true iff i is prime           */
static bool   *is_twin    = NULL;/* is_twin[i] == true iff i is in a twin pair   */
static uint32_t *prime_list = NULL;/* list of primes up to sieve_limit */
static size_t   prime_count   = 0; /* number of primes stored */

/* ----------------------------------------------------------------------
 * Build/extend a sieve up to 'limit' (inclusive).
 * ---------------------------------------------------------------------- */
static void build_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;   /* already sufficient */

    /* free old tables */
    free(is_prime);
    free(is_twin);
    free(prime_list);

    /* allocate new tables */
    is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    is_twin  = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime || !is_twin) return;   /* allocation failure */

    /* Mark all numbers >= 2 as potential primes */
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;

    /* Eratosthenes sieve */
    for (idx_t p = 2; (idx_t)p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = false;
        }
    }

    /* Mark twin‑prime flags */
    for (idx_t p = 3; p <= limit; p += 2) {
        if (!is_prime[p]) continue;
        if (p - 2 >= 2 && is_prime[p - 2]) is_twin[p] = true;
        if (p + 2 <= limit && is_prime[p + 2]) is_twin[p] = true;
    }

    /* Count primes for allocation of prime_list */
    prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++prime_count;

    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) return;   /* allocation failure */

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            prime_list[idx++] = (uint32_t)i;
        }
    }

    sieve_limit = limit;
}

/* ----------------------------------------------------------------------
 * Ensure the cache can hold entries up to (and including) 'needed-1'.
 * ---------------------------------------------------------------------- */
static void ensure_cache_size(idx_t needed)
{
    if (needed <= cache_size) return;
    size_t new_size = (size_t)needed;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) return;   /* allocation failure */

    if (cache) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache        = new_cache;
    cache_size   = new_size;
}

/* ----------------------------------------------------------------------
 * Compute the sequence values for all k in [max_n_cached+1, n].
 * ---------------------------------------------------------------------- */
static void compute_caches_up_to(idx_t n)
{
    idx_t start = max_n_cached + 1;
    if (start > n) return;

    /* Allocate cache if necessary */
    ensure_cache_size(n + 1);   /* indices 0 .. n inclusive */

    for (idx_t k = start; k <= n; ++k) {
        seq_t count = 0;
        idx_t target = k * 2;    /* 2k */

        for (size_t pi = 0; pi < prime_count; ++pi) {
            uint32_t p_u32 = prime_list[pi];
            idx_t p = (idx_t)p_u32;

            if (p > target / 2) break;   /* q would be smaller than p */
            idx_t q = target - p;
            if (q < p) continue;         /* keep unordered pairs only */
            if (q > sieve_limit) continue;

            if (!is_prime[(size_t)q]) continue;
            if (is_twin[(size_t)p] || is_twin[(size_t)q]) {
                ++count;
            }
        }
        cache[k] = count;
    }
    max_n_cached = n;
}

/* ----------------------------------------------------------------------
 * Public sequence function
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n < 1 */

    /* Ensure the sieve covers 2n */
    build_sieve(n * 2);

    /* Compute all required values up to n */
    if (n > max_n_cached) {
        compute_caches_up_to(n);
    }
    return cache[n];
}
