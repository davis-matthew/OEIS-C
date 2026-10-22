/*
 * A379316  Positive integers whose prime indices include a unique squarefree
 *          number.
 * https://oeis.org/A379316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer whose prime indices * contain
 *            exactly one square‑free index. * The prime index of a prime p is
 *            the position of p in the list * of all primes (2,3,5,7,11,…). *
 *            * The implementation factors each candidate using a pre‑computed
 *            * prime list up to a fixed limit (2 000 0...
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   build_primes, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"
#include "../utils/is_squarefree.h"

/* ------------------------------------------------------------------ */
/* Prime generation (once)                                            */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list = NULL; /* sorted primes */
static size_t     prime_cnt  = 0;   /* number of primes */

/* Generate all primes up to LIMIT immediately on first use. */
static void init_primes(void)
{
    if (prime_list != NULL)
        return; /* already done */

    /* LIMIT large enough for candidates up to n ≈ 10000. */
    const idx_t LIMIT = 2000000; /* two million */

    /* build_primes(limit, &array, &count) allocates and returns
     * an array of primes ≤ limit. */
    build_primes(LIMIT, &prime_list, &prime_cnt);
}

/* ------------------------------------------------------------------ */
/* Helper: convert a prime to its 1‑based index.                        */
/* ------------------------------------------------------------------ */
static inline size_t prime_index_of(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < prime_cnt && prime_list[lo] == p) ? (lo + 1) : 0; /* 0 → not found */
}

/* ------------------------------------------------------------------ */
/* Simple trial–division factorisation using the prime list.           */
/* ------------------------------------------------------------------ */
static void factor_uint64(uint64_t n, uint64_t pr[64], uint64_t ex[64], size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= n; ++i) {
        uint64_t p = prime_list[i];
        if (n % p == 0) {
            uint64_t e = 0;
            do {
                ++e;
                n /= p;
            } while (n % p == 0);
            pr[(*cnt)] = p;
            ex[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {            /* remaining prime factor */
        pr[(*cnt)] = n;
        ex[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* ------------------------------------------------------------------ */
/* Check whether a candidate satisfies the unique square‑free rule.    */
/* ------------------------------------------------------------------ */
static bool good_candidate(uint64_t val)
{
    /* factor val */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_uint64(val, pr, ex, &cnt);

    int squarefree_hits = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        size_t idx = prime_index_of(p);
        if (idx == 0)
            return false;                     /* prime outside our table -> ignore */

        if (is_squarefree((seq_t)idx)) {     /* idx is square‑free */
            if (e == 1)
                ++squarefree_hits;
            else
                return false;                 /* repeated square‑free index */
        }
    }

    return squarefree_hits == 1;
}

/* ------------------------------------------------------------------ */
/* Cache for already computed terms                                     */
/* ------------------------------------------------------------------ */
static seq_t *cache     = NULL;  /* array of seq_t values */
static size_t cache_len = 0;     /* number of entries currently stored */
static size_t cache_cap = 0;     /* capacity of cache */

/* ------------------------------------------------------------------ */
/* Public sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined, but guard */

    init_primes();                   /* ensure prime list */

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Grow cache to at least a capacity of new entries. */
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : (cache_cap << 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);                  /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Start checking from the next integer after the last cached term. */
    uint64_t candidate = (cache_len == 0) ? 2 : (uint64_t)cache[cache_len - 1] + 1;

    while (cache_len < (size_t)n) {
        if (good_candidate(candidate)) {
            cache[cache_len++] = (seq_t)candidate;
        }
        ++candidate;
    }

    return cache[n - 1];
}
