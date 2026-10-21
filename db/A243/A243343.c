/*
 * A243343  a(1)=1; thereafter, if n is the k-th squarefree number (i.e., n =
 *          A005117(k)), a(n) = 1 + (2*a(k-1)); otherwise, when n is k-th
 *          nonsquarefree number (i.e., n = A013929(k)), a(n) = 2*a(k).
 * https://oeis.org/A243343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 2314, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(1) = 1
 *   For n > 1:
 *     let k be the rank of n among squarefree numbers (n = A005117(k)).
 *       a(n) = 1 + 2 * a(k-1)
 *     else let k be the rank of n among nonsquarefree numbers (n = A013929(k)).
 *       a(n) = 2 * a(k)
 *
 * The implementation caches all computed values and a smallest‑prime‑factor
 * table to test squarefreeness in O(log n).  It works for any `seq_t` &
 * `idx_t` types supplied by the build and compiles under both C11 and
 * C++17.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* ----- static state for caching ----- */
    static seq_t *cache      = NULL;  /* cache[1..max] holds a(i) */
    static idx_t cache_cap   = 0;     /* allocated size */
    static idx_t cache_len   = 0;     /* highest index computed */

    static idx_t sf_cnt      = 0;     /* number of squarefree numbers seen */
    static idx_t nf_cnt      = 0;     /* number of nonsquarefree numbers seen */

    static idx_t *spf        = NULL;  /* smallest‑prime‑factor table */
    static idx_t spf_limit   = 0;     /* highest n for which spf is valid */

    /* ----- base case ----- */
    if (n == 1) {
        if (cache_len < 1) {          /* initialise cache */
            cache = (seq_t*)malloc(2 * sizeof(seq_t)); /* indices 0..1 */
            cache_cap = 2;
            cache[1] = 1;
            cache_len = 1;
            sf_cnt = 1;  /* 1 is squarefree */
            nf_cnt = 0;
        }
        return cache[1];
    }

    /* ----- expand cache if needed ----- */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)(n + 1);      /* +1 because cache[0] unused */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* ----- build spf table up to n ----- */
    if (n > spf_limit) {
        free(spf);
        spf_limit = n;
        spf = (idx_t*)malloc((size_t)(spf_limit + 1) * sizeof(idx_t));
        for (idx_t i = 0; i <= spf_limit; ++i) spf[i] = 0;
        for (idx_t i = 2; i * i <= spf_limit; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                for (idx_t j = i * i; j <= spf_limit; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
        for (idx_t i = 2; i <= spf_limit; ++i)
            if (spf[i] == 0) spf[i] = i;
    }

    /* ----- compute missing values up to n ----- */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* test squarefreeness using spf table */
        bool squarefree = true;
        idx_t x = i;
        while (x > 1) {
            idx_t p = spf[x];
            idx_t exp = 0;
            do {
                x /= p;
                ++exp;
            } while (x % p == 0);
            if (exp > 1) { squarefree = false; break; }
        }

        if (squarefree) {
            ++sf_cnt;                       /* this is the sf_cnt‑th squarefree */
            idx_t k = sf_cnt;
            cache[i] = 1 + 2 * cache[k - 1]; /* a(k-1) already computed */
        } else {
            ++nf_cnt;                       /* this is the nf_cnt‑th nonsquarefree */
            idx_t k = nf_cnt;
            cache[i] = 2 * cache[k];
        }
    }

    cache_len = n;
    return cache[n];
}
