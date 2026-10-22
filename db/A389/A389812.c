/*
 * A389812  Numbers whose prime indices are not in arithmetic progression.
 * https://oeis.org/A389812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   compare_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A389812 ---------------------------------------------------------
   Numbers whose prime indices are not in arithmetic progression.
   The function generates terms sequentially using an on‑demand SPF
   sieve and caches them so that any call to a(n) (1‑based) is fast.
   The algorithm is a brute force search, but with a efficient factoriser
   and a simple AP test.                                                    */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/compare_idx.h"

/* --------------------------------------------------------------------- */
/*  Global data                                                         */
static idx_t *spf            = NULL;  /* smallest prime factor */
static idx_t *prime_index    = NULL;  /* 1‑based index of each prime */
static idx_t  sieve_limit    = 0;     /* maximum value sieved */

/* --------------------------------------------------------------------- */
/*  Cache for the sequence                                               */
static seq_t *cache          = NULL;  /* array of computed terms */
static size_t cache_len      = 0;     /* number of stored terms   */
static size_t cache_cap      = 0;     /* allocated capacity       */

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/*  Build SPF and prime indices up to 'limit' (inclusive).               */
static void init_sieve_up_to(idx_t limit)
{
    /* free old data if any */
    free(spf);
    free(prime_index);

    /* allocate new arrays (size_t is safe for memory size) */
    spf          = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    prime_index  = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!spf || !prime_index)
        return;   /* allocation failed, caller must handle */

    /* initialise spf */
    for (idx_t i = 0; i <= limit; ++i) spf[i] = i;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }

    /* determine prime indices (1‑based) */
    idx_t pos = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == i) {
            prime_index[i] = pos++;
        } else {
            prime_index[i] = 0;
        }
    }
    sieve_limit = limit;
}

/* --------------------------------------------------------------------- */
/*  Ensure the cache can hold at least 'need' terms.                    */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : need;
    if (new_cap < need) new_cap = need;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                               */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* out of bounds, harmless */

    /* Start sieving from a modest limit and grow as needed. */
    static idx_t cur_num = 2;         /* next integer to test */

    /* Ensure cache is sufficient for requested index. */
    ensure_cache_cap((size_t)n);

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Extend sieve if cur_num exceeds current limit. */
        if (cur_num > sieve_limit) {
            idx_t new_limit = sieve_limit ? sieve_limit * 2 : 32;
            while (new_limit < cur_num) new_limit *= 2;
            init_sieve_up_to(new_limit);
        }

        /* Factor cur_num using SPF and collect prime indices. */
        idx_t tmp      = cur_num;
        idx_t indices[32];
        size_t mlen = 0;

        while (tmp > 1) {
            idx_t p  = spf[tmp];
            idx_t idx = prime_index[p];
            indices[mlen++] = idx;
            tmp /= p;
        }

        /* Test whether the indices form an arithmetic progression. */
        bool include = false;
        if (mlen >= 2) {
            qsort(indices, mlen, sizeof(idx_t), compare_idx);
            idx_t d = indices[1] - indices[0];
            bool ap = true;
            for (size_t i = 2; i < mlen; ++i) {
                if (indices[i] - indices[i-1] != d) {
                    ap = false;
                    break;
                }
            }
            if (!ap) include = true;
        }

        /* Store the term if it passes the test. */
        if (include) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)cur_num;
        }

        ++cur_num;
    }

    return cache[n - 1];
}
