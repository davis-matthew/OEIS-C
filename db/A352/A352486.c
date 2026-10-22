/*
 * A352486  Heinz numbers of non-self-conjugate integer partitions.
 * https://oeis.org/A352486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A352486 (Heinz numbers of non‑self‑conjugate integer partitions) */
/* A(n) is the n-th smallest integer whose partition (obtained from the
   prime‑index factorisation) is not self‑conjugate.  The sequence is
   computed by scanning the positive integers, testing whether the
   corresponding partition is self‑conjugate, and caching the results
   for repeated calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>        /* for sqrt() */

static uint32_t *g_primes          = NULL;  /* prime list   */
static size_t    g_prime_count     = 0;     /* number of primes stored   */
static size_t    g_prime_cap       = 0;     /* current allocated capacity */
static uint32_t *g_prime_index_map = NULL;  /* prime -> 1‑based index, 0 if not prime */
static uint32_t  g_prime_limit     = 0;     /* largest prime value generated */

/* ------------------------------------------------------------------ */
/* Build a simple sieve of Eratosthenes up to `limit` (inclusive).    */
/* ------------------------------------------------------------------ */
static void build_primes_upto(uint32_t limit)
{
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(limit + 1));
    if (!is_prime) abort();
    memset(is_prime, 1, (size_t)(limit + 1));
    is_prime[0] = is_prime[1] = 0;

    uint32_t i, j;
    uint32_t sqrt_limit = (uint32_t)sqrt((double)limit);
    for (i = 2; i <= sqrt_limit; ++i) {
        if (is_prime[i]) {
            for (j = i * i; j <= limit; j += i) is_prime[j] = 0;
        }
    }

    /* Allocate or resize the primes array. */
    size_t cap = limit / 10 + 100;
    if (cap > UINT32_MAX) cap = UINT32_MAX;
    if (g_primes) free(g_primes);
    g_primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!g_primes) { free(is_prime); abort(); }
    g_prime_count = 0;
    g_prime_cap   = cap;

    /* Allocate or resize the index map array. */
    if (g_prime_index_map) free(g_prime_index_map);
    g_prime_index_map = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    if (!g_prime_index_map) { free(is_prime); abort(); }

    for (i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (g_prime_count == g_prime_cap) {
                g_prime_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(g_primes, g_prime_cap * sizeof(uint32_t));
                if (!tmp) { free(is_prime); abort(); }
                g_primes = tmp;
            }
            g_primes[g_prime_count] = i;
            ++g_prime_count;
            g_prime_index_map[i] = (uint32_t)g_prime_count; /* 1‑based index */
        }
    }

    free(is_prime);
    g_prime_limit = limit;
}

/* Ensure that primes up to at least `need` are available. */
static void ensure_prime_limit(uint64_t need)
{
    if (need <= g_prime_limit) return;
    if (need > 50000000ULL) need = 50000000ULL;   /* guard against huge limits */
    build_primes_upto((uint32_t)need);
}

/* Return the 1‑based index of prime `p`.  The prime list is extended
   if necessary. */
static uint32_t prime_index(uint32_t p)
{
    if (p > g_prime_limit) ensure_prime_limit((uint64_t)p);
    return g_prime_index_map[p];
}

/* ------------------------------------------------------------------ */
/* Determine whether the partition corresponding to `m` is self‑conjugate. */
/* ------------------------------------------------------------------ */
static bool is_self_conjugate_u64(uint64_t m)
{
    if (m <= 1) return false;   /* partition of 0? exclude */

    /* Accumulate prime indices and exponents.  At most 64 distinct primes
       because 2^64 > 10^19.   */
    uint32_t idxs[64];
    uint32_t exps[64];
    size_t nf = 0;

    uint64_t t = m;
    for (size_t pi = 0; pi < g_prime_count && (uint64_t)g_primes[pi] * g_primes[pi] <= t; ++pi) {
        uint64_t p = g_primes[pi];
        if (t % p == 0) {
            uint32_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            if (e) {
                idxs[nf] = prime_index((uint32_t)p);
                exps[nf] = e;
                ++nf;
            }
        }
    }

    if (t > 1) {          /* remaining prime factor */
        uint32_t p = (uint32_t)t;
        uint32_t idx = prime_index(p);
        if (idx == 0) return false;          /* should not happen */
        idxs[nf] = idx;
        exps[nf] = 1;
        ++nf;
    }

    /* Total number of parts, and build the multiset of part indices. */
    uint32_t total_parts = 0;
    for (size_t k = 0; k < nf; ++k) total_parts += exps[k];

    /* The part multiset can fit in 64 slots because total_parts <= log2(m). */
    int parts[128];
    size_t pos = 0;
    for (size_t k = 0; k < nf; ++k) {
        for (uint32_t e = 0; e < exps[k]; ++e) {
            parts[pos++] = (int)idxs[k];
        }
    }

    /* Sort in non‑increasing order. */
    for (size_t i = 0; i < pos; ++i)
        for (size_t j = i + 1; j < pos; ++j)
            if (parts[i] < parts[j]) {
                int tmp = parts[i];
                parts[i] = parts[j];
                parts[j] = tmp;
            }

    /* Test the self‑conjugate condition. */
    for (uint32_t j = 1; j <= total_parts; ++j) {
        uint32_t f = 0;
        for (size_t k = 0; k < nf; ++k)
            if (idxs[k] >= j) f += exps[k];
        if ((uint32_t)parts[j - 1] != f) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Cache of previously computed terms.                                 */
/* ------------------------------------------------------------------ */
static seq_t *g_cache          = NULL;
static size_t g_cache_cap      = 0;   /* allocated slots */
static size_t g_cache_size     = 0;   /* number of cached terms */
static uint64_t g_last_checked = 2;   /* last integer examined */

/* Grow cache array to at least `required` elements. */
static void grow_cache(size_t required)
{
    if (required <= g_cache_cap) return;
    size_t new_cap = g_cache_cap ? g_cache_cap * 2 : 8;
    while (new_cap < required) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    g_cache     = tmp;
    g_cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Public sequence function.                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initialise primes if not done yet. */
    if (g_prime_limit == 0) build_primes_upto(2000000U);   /* reasonable start */

    if ((size_t)n > g_cache_size) {
        grow_cache((size_t)n);
        while (g_cache_size < (size_t)n) {
            ++g_last_checked;
            if (!is_self_conjugate_u64(g_last_checked)) {
                g_cache[g_cache_size++] = (seq_t)g_last_checked;
            }
        }
    }
    return g_cache[n - 1];
}
