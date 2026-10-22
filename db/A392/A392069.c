/*
 * A392069  Powers k^m divisible by only 1 prime cube, with m > 1 and
 *          nonsquarefree weak k (in A332785).
 * https://oeis.org/A392069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (k_n)^2, where k_n is the n-th integer k > 1 that
 *            contains exactly one prime factor with exponent ≥ 2 and at least
 *            one additional distinct prime factor. That is, k = p^a · q₁ · …
 *            · q_t with a ≥ 2, each q_i ≠ p, a > 1 and t ≥ 1. The sequence is
 *            the sorted list of k²
 * Range:     u8 n <= 1, u16 n <= 72, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The functions and types below are supplied by the build system. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* cache of sequence values, 0‑based */
static seq_t *seq_cache = NULL;
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* allocated capacity    */

/* Smallest prime factor array, inclusive up to spf_limit. */
static uint32_t *spf        = NULL;
static size_t     spf_limit = 0;   /* current upper limit (inclusive) */

/* Construct or extend the SPF table to cover all numbers up to LIMIT
   (inclusive).  The table is rebuilt from scratch, which is fine
   because LIMIT grows only by a factor of two each time. */
static void build_spf(size_t limit)
{
    uint32_t *new_spf = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
    if (!new_spf) exit(1);
    spf = new_spf;
    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {           /* i is prime */
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }
    spf_limit = limit;
}

/* Test whether k satisfies the required factor pattern:
   exactly one prime with exponent ≥ 2 and at least one other
   distinct prime factor.  (k must be ≤ spf_limit.) */
static bool valid_k(idx_t k)
{
    int repeated = 0;   /* how many primes have exponent ≥ 2 */
    int distinct = 0;   /* number of distinct prime factors   */

    while (k > 1) {
        uint32_t p = spf[(size_t)k];
        int cnt = 0;
        while (k % p == 0) {
            k /= p;
            ++cnt;
        }
        if (cnt > 1) ++repeated;
        ++distinct;
    }
    return (repeated == 1 && distinct >= 2);
}

/* Ensure that seq_cache contains at least NEED terms 
   (with index 1‑based).  The search for new terms continues from
   the last candidate k processed by the previous call. */
static void ensure_cache(idx_t need)
{
    /* Initialise SPF table once, large enough for the usual range. */
    if (spf == NULL) {
        build_spf(200000);   /* initial size – plenty for n up to 10⁴ */
    }

    /* Candidate integer k, starting from 2 the first time. */
    static idx_t k = 2;

    while (cache_size < (size_t)need) {
        /* Expand SPF table if we have run out of precomputed range. */
        if (k > (idx_t)spf_limit) {
            size_t new_limit = spf_limit * 2 + 1000;
            build_spf(new_limit);
        }

        if (valid_k(k)) {
            /* Grow the cache array as needed. */
            if (cache_size + 1 > cache_cap) {
                size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                seq_cache = tmp;
                cache_cap = new_cap;
            }
            seq_cache[cache_size++] = (seq_t)k * (seq_t)k;
        }
        ++k;
    }
}

/* OEIS sequence A392069. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined entry – return safe value */
    ensure_cache(n);
    return seq_cache[n - 1];       /* sequence is 1‑based, cache is 0‑based */
}
