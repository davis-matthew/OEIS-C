/*
 * A396597  Numbers m with exactly two distinct prime factors q and p such
 *          that m = q^k * p^j and p > q^k with k, j >= 1.
 * https://oeis.org/A396597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) returns the n-th integer that has exactly two distinct
 *            prime factors q and p, written as m = q^k * p^j (k,j ≥ 1) with
 *            the condition p > q^k. The sequence is sorted increasingly. This
 *            implementation generates candidates using primes up to a
 *            dynamically increased limit, sorts them, removes dup...
 * Range:     u8 none, u16 none, u32 n <= 6777, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>    /* for pow if needed in other environments */

#include "../utils/build_primes_upto.h"

/* --------------------------------------------------------------------- */
/* comparison routine for qsort ---------------------------------------- */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* --------------------------------------------------------------------- */
/* global cache, capacity and length ---------------------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* --------------------------------------------------------------------- */
/* expand the global cache until it contains at least target_n elements  */
static void build_cache(idx_t target_n)
{
    if (cache != NULL) return;              /* already built once */

    /* start with a modest limit and double until we have enough terms */
    uint64_t limit = 1ULL << 20;            /* 1,048,576 */
    while (1) {
        /* get primes up to 'limit' */
        uint64_t *primes;
        size_t p_cnt;
        build_primes_upto(limit, &primes, &p_cnt);

        /* estimate required temporary capacity */
        size_t cap = p_cnt * 8;             /* generous over‑estimate */
        seq_t *tmp = (seq_t *)calloc(cap, sizeof(seq_t));
        size_t tmplen = 0;

        for (size_t i = 0; i < p_cnt; ++i) {
            uint64_t q = primes[i];
            seq_t qpow = (seq_t)q;
            for (int k = 1; ; ++k, qpow *= (seq_t)q) {
                if (qpow > (seq_t)(limit / 2)) break;   /* next p > qpow would overflow limit */

                /* find first prime greater than qpow */
                size_t j = i + 1;
                while (j < p_cnt && primes[j] <= (uint64_t)qpow) ++j;

                for (; j < p_cnt; ++j) {
                    uint64_t p = primes[j];
                    /* Ensure p * qpow fits in limit before any power of p */
                    seq_t prod = qpow * (seq_t)p;
                    if (prod > (seq_t)limit) break;

                    seq_t pow = prod;
                    for (; pow <= (seq_t)limit; ) {
                        /* store candidate */
                        if (tmplen >= cap) {
                            size_t new_cap = cap * 2;
                            seq_t *new_tmp = (seq_t *)realloc(tmp, new_cap * sizeof(seq_t));
                            if (new_tmp == NULL) abort();
                            tmp = new_tmp;
                            cap = new_cap;
                        }
                        tmp[tmplen++] = pow;

                        /* next power of p */
                        if (pow > (seq_t)(limit / p)) break;
                        pow *= (seq_t)p;
                    }
                }
            }
        }

        free(primes);

        /* sort and deduplicate */
        qsort(tmp, tmplen, sizeof(seq_t), cmp_seq_t);

        size_t uniq_cap = tmplen;
        seq_t *uniq = (seq_t *)malloc(uniq_cap * sizeof(seq_t));
        size_t uniq_len = 0;
        if (tmplen > 0) {
            uniq[uniq_len++] = tmp[0];
            for (size_t t = 1; t < tmplen; ++t) {
                if (tmp[t] != tmp[t-1]) uniq[uniq_len++] = tmp[t];
            }
        }

        free(tmp);
        cache = uniq;
        cache_len = uniq_len;
        cache_cap = uniq_cap;

        if (cache_len >= (size_t)target_n) break;

        /* not enough terms – increase limit and try again */
        free(cache);
        cache = NULL;
        cache_len = 0;
        cache_cap = 0;
        limit <<= 1;   /* double the limit */
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 0) return 0;          /* offset is 1-based */

    if (cache == NULL) build_cache(n);

    /* If the cache is too small, enlarge it (rarely happens). */
    if (n > (idx_t)cache_len)
        build_cache(n * 2);

    return cache[(size_t)n - 1];
}
