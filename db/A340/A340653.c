/*
 * A340653  Number of balanced factorizations of n.
 * https://oeis.org/A340653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered factorizations of n into factors >1
 *            such that the length of the factorization equals the maximum
 *            Ω(f) of its factors. The empty factorization is counted for n=1
 * Range:     u8 n <= 9997, u16 n <= 9997, u32 n <= 9997, u64 n <= 9997,
 *            u128 n <= 9997, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#if __cplusplus
#  define CCAST(p, type) static_cast<type>(p)
#else
#  define CCAST(p, type) ((type)(p))
#endif

/* External types supplied by the build system. */
extern seq_t a(idx_t n);

/* Helper tables for Ω(n) computation. */
static idx_t *spf = NULL;    /* smallest prime factor  */
static idx_t *omega = NULL;  /* Ω(n): number of prime factors with multiplicity */
static idx_t spf_cap = 0;    /* current capacity of spf and omega arrays */

/* Cache for previously computed a(n). */
static seq_t *cache_vals = NULL;
static unsigned char *cache_seen = NULL;
static idx_t cache_cap = 0;

/* Ensure that spf[0..n] and omega[0..n] are available. */
static void precompute(idx_t n)
{
    if (n < spf_cap)
        return;

    idx_t new_cap = n + 1;
    idx_t *new_spf = CCAST(realloc(spf, new_cap * sizeof(idx_t)), idx_t *);
    if (!new_spf) exit(1);
    spf = new_spf;

    idx_t *new_omega = CCAST(realloc(omega, new_cap * sizeof(idx_t)), idx_t *);
    if (!new_omega) exit(1);
    omega = new_omega;

    /* Initialise spf entries to 0 (primes will be set below). */
    for (idx_t i = 2; i <= n; ++i)
        spf[i] = 0;

    /* Sieve for smallest prime factors. */
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i * i <= n)
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
        }
    }

    /* Compute Ω(n): Ω(1)=0, Ω(m)=Ω(m/spf[m])+1. */
    omega[0] = 0;
    omega[1] = 0;
    for (idx_t i = 2; i <= n; ++i)
        omega[i] = omega[i / spf[i]] + 1;

    spf_cap = new_cap;
}

/* Ensure that the cache covers indices up to n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;

    idx_t new_cap = n + 1;
    seq_t *new_vals = CCAST(realloc(cache_vals, new_cap * sizeof(seq_t)), seq_t *);
    if (!new_vals) exit(1);
    cache_vals = new_vals;

    unsigned char *new_seen = CCAST(realloc(cache_seen, new_cap * sizeof(unsigned char)),
                                   unsigned char *);
    if (!new_seen) exit(1);
    cache_seen = new_seen;

    for (idx_t i = cache_cap; i < new_cap; ++i)
        cache_seen[i] = 0;

    cache_cap = new_cap;
}

/* Recursive enumeration of unordered factorizations.
   min_factor ensures non‑decreasing order to avoid permutations. */
static seq_t dfs(idx_t n, idx_t min_factor,
                 idx_t curr_len, idx_t curr_maxOmega)
{
    if (n == 1)
        return (curr_len == curr_maxOmega) ? (seq_t)1 : (seq_t)0;

    seq_t total = 0;

    /* Try proper divisors d >= min_factor, with d <= sqrt(n). */
    for (idx_t d = min_factor; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t dOmega = omega[d];
            total += dfs(n / d, d, curr_len + 1,
                         (dOmega > curr_maxOmega) ? dOmega : curr_maxOmega);
        }
    }

    /* Option: take the factor n itself. */
    if (n >= min_factor) {
        idx_t nOmega = omega[n];
        total += dfs(1, n, curr_len + 1,
                     (nOmega > curr_maxOmega) ? nOmega : curr_maxOmega);
    }

    return total;
}

/* Required external function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    precompute(n);
    ensure_cache(n);

    if (cache_seen[n])
        return cache_vals[n];

    seq_t result = dfs(n, 2, 0, 0);

    cache_vals[n] = result;
    cache_seen[n] = 1;

    return result;
}
