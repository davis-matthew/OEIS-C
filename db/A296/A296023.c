/*
 * A296023  Numbers n such that there is precisely 1 group of order n and 2 of
 *          order n + 1.
 * https://oeis.org/A296023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1663, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 70 terms through n = 1797 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296023: numbers n such that there is exactly 1 group of order n and
   2 of order n + 1.
   The code generates candidates by testing the conditions
   A000001(n) = 1  (cyclic)  and  A000001(n+1) = 2
   and caches the found terms so that subsequent calls are O(1). */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* ------------------------------------------------------------------ */
static bool has_two_groups_u64(uint64_t n)
{
    if (n == 4)                     /* 2² also has 2 groups   */
        return true;

    /* factorisation */
    uint64_t primes[64];
    int      exps[64];
    int      cnt = 0;
    uint64_t m   = n;

    for (uint64_t d = 2; d <= m / d; ++d) {
        if (m % d == 0) {
            int e = 0;
            while (m % d == 0) {
                m /= d;
                ++e;
            }
            primes[cnt] = d;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (m > 1) {
        primes[cnt] = m;
        exps[cnt]   = 1;
        ++cnt;
    }

    if (cnt == 1) {                /* prime power n = p^k      */
        return exps[0] == 2;       /* only p² gives 2 groups    */
    }

    if (cnt == 2 && exps[0] == 1 && exps[1] == 1) {
        /* n = p*q  (distinct primes).
           Two groups iff p | (q−1). */
        uint64_t p = primes[0];
        uint64_t q = primes[1];
        if (p > q) {                /* ensure p < q */
            uint64_t t = p; p = q; q = t;
        }
        return ((q - 1) % p) == 0;
    }

    return false;                  /* all other cases */
}

/* ------------------------------------------------------------------ */
static bool is_cyclic_u64(uint64_t n)
{
    if (n <= 3)                       /* 1,2,3 are cyclic */
        return true;

    /* factor n and check conditions: square‑free and p∤q‑1 for all primes p<q */
    uint64_t primes[64];
    int      cnt = 0;
    uint64_t m   = n;
    for (uint64_t d = 2; d <= m / d; ++d) {
        if (m % d == 0) {
            int exp = 0;
            while (m % d == 0) {
                m /= d;
                ++exp;
            }
            if (exp > 1)                 /* not square‑free */
                return false;
            primes[cnt++] = d;
        }
    }
    if (m > 1) {                       /* remaining prime factor  */
        primes[cnt++] = m;
    }

    /* check p∤q‑1 for all pairs */
    for (int i = 0; i < cnt; ++i) {
        for (int j = i + 1; j < cnt; ++j) {
            uint64_t p = primes[i];
            uint64_t q = primes[j];
            if ((q % p) == 1 || (p % q) == 1)
                return false;
        }
    }
    return true;
}

static seq_t *cached_terms = NULL;   /* found terms */
static size_t   cache_len   = 0;      /* number of stored terms   */
static size_t   cache_cap   = 0;      /* allocated capacity      */
static uint64_t last_checked = 0;     /* largest integer already tested */

/* ------------------------------------------------------------------ */
/*  Helper: grow the cached_terms array.                             */
/* ------------------------------------------------------------------ */
static void grow_cache(size_t new_cap)
{
    seq_t *tmp = mem_resize(seq_t, cached_terms, new_cap);
    if (!tmp) abort();                /* allocation failed  */
    cached_terms = tmp;
    cache_cap    = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Helper: test whether n (uint64_t) is cyclic (exactly one group). */

/* ------------------------------------------------------------------ */
/*  Helper: test whether n (uint64_t) has exactly two groups.        */

/* ------------------------------------------------------------------ */
/*  Ensure that at least 'need' terms have been generated.           */
/* ------------------------------------------------------------------ */
static void ensure_terms(idx_t need)
{
    while (cache_len < (size_t)need) {
        ++last_checked;                    /* next candidate */

        /* test candidate: A000001(last_checked) = 1  &&
                            A000001(last_checked+1) = 2 */
        uint64_t cand = last_checked;
        if (is_cyclic_u64(cand) &&
            has_two_groups_u64(cand + 1)) {

            /* store term */
            if (cache_len == cache_cap)
                grow_cache(cache_cap ? cache_cap * 2 : 4);

            cached_terms[cache_len++] = (seq_t)cand;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence function.                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                      /* undefined for n < 1   */
        return 0;

    ensure_terms(n);                  /* compute up to n-th term */
    return cached_terms[n - 1];       /* OEIS uses 1‑based offset */
}
