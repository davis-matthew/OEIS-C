/*
 * A303283  Squarefree numbers whose prime indices have no common divisor
 *          other than 1 but are not pairwise coprime.
 * https://oeis.org/A303283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute-force search of squarefree numbers whose prime
   indices have no common divisor other than 1 but are not pairwise
   coprime. */
/* This function is memoised; it works for any sequence term requested
   in any order. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Prime table logic -------------------------------------------------- */
/* ------------------------------------------------------------------ */

#define PRIME_LIMIT 20000000U   /* enough to cover numbers searched up to ~10⁷ */

static uint32_t *primes      = NULL;   /* array of primes (0‑based) */
static size_t   prime_cnt    = 0;      /* number of primes stored */
static bool     primes_ready = false;

/* Simple Euclidean GCD for 32‑bit values. */
static uint32_t gcd32(uint32_t a, uint32_t b)
{
    while (b != 0) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Return the 1‑based index of a prime p.  Assumes p is present in the
   primes array. */
static uint32_t prime_index(uint32_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* primes is 0‑based but indices are 1‑based */
    return (uint32_t)(lo + 1);
}

/* Sieve of Eratosthenes up to PRIME_LIMIT, storing primes in the
   global array.  Called once on first use. */
static void build_primes(void)
{
    uint32_t limit = PRIME_LIMIT;
    bool *is_composite = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    size_t cap = limit / 10;          /* initial generous capacity */
    uint32_t *tmp   = (uint32_t *)malloc(cap * sizeof(uint32_t));
    size_t cnt = 0;

    for (uint32_t i = 2; i <= limit; ++i) {
        if (!is_composite[(size_t)i]) {
            if (cnt == cap) {        /* grow array */
                cap <<= 1;
                tmp = (uint32_t *)realloc(tmp, cap * sizeof(uint32_t));
            }
            tmp[cnt++] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                    is_composite[(size_t)j] = true;
            }
        }
    }
    free(is_composite);

    primes      = tmp;
    prime_cnt   = cnt;
    primes_ready = true;
}

/* ------------------------------------------------------------------ */
/* Helper to factor a uint64_t number and gather the indices of its
   distinct prime factors.  The array indices[] is written and the count
   is returned.  It returns false if the number is not squarefree. */
static bool factor_indices(uint64_t n, uint32_t *indices, size_t *k)
{
    *k = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        if (n % p == 0) {
            /* found a factor p */
            n /= p;
            if (n % p == 0) {            /* squared factor -> not squarefree */
                return false;
            }
            indices[(*k)++] = prime_index(p);
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1) {                        /* remaining prime factor > sqrt(n) */
        if (n <= UINT32_MAX) {
            indices[(*k)++] = prime_index((uint32_t)n);
        } else {
            /* The factor is a huge prime not in our sieve; compute
               its index by linear probing the list of primes. */
            bool found = false;
            for (size_t i = 0; i < prime_cnt; ++i) {
                if ((uint64_t)primes[i] == n) {
                    indices[(*k)++] = (uint32_t)(i + 1);
                    found = true;
                    break;
                }
            }
            if (!found) return false;   /* shouldn't happen with large limit */
        }
    }
    return true;                        /* squarefree */
}

/* Test whether a number satisfies the OEIS condition. */
static bool qualifies(uint64_t num)
{
    uint32_t indices[32];
    size_t   k;
    if (!factor_indices(num, indices, &k))
        return false;                    /* not squarefree */

    if (k == 0)
        return false;                    /* number 1 */

    /* compute gcd of all indices */
    uint32_t g = indices[0];
    for (size_t i = 1; i < k; ++i)
        g = gcd32(g, indices[i]);

    if (g != 1)
        return false;                    /* common divisor >1 */

    /* ensure at least one pair of indices is not coprime */
    bool not_pairwise = false;
    for (size_t i = 0; i < k && !not_pairwise; ++i) {
        for (size_t j = i + 1; j < k; ++j) {
            if (gcd32(indices[i], indices[j]) > 1) {
                not_pairwise = true;
                break;
            }
        }
    }
    return not_pairwise;
}

/* ------------------------------------------------------------------ */
/* Sequence cache ----------------------------------------------------- */
/* ------------------------------------------------------------------ */

static seq_t *cache       = NULL;   /* array of terms */
static size_t  cache_len  = 0;      /* number of stored terms */
static size_t  cache_cap  = 0;      /* allocated capacity */
static uint64_t search_start = 2;   /* next integer to test */

/* Grow cache to at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) return;           /* allocation failure – keep old cache */
    cache = new_arr;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main function a(idx_t n) ------------------------------------------ */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n=0, but safe */

    if (!primes_ready)
        build_primes();

    /* Expand cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* Find next qualifying number */
        uint64_t cand = search_start;
        while (!qualifies(cand))
            ++cand;

        /* Ensure capacity */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 4;
            grow_cache(new_cap);
        }

        cache[cache_len++] = (seq_t)cand;
        search_start = cand + 1;
    }

    return cache[(size_t)n - 1];
}
