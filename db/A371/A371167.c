/*
 * A371167  Positive integers with more divisors (A000005) than distinct
 *          divisors of prime indices (A370820).
 * https://oeis.org/A371167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer m such that divisor count of m
 *            > the number of distinct divisors of the indices of the primes
 *            dividing m (counted with multiplicity)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   build_primes_upto, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */
/*  Dependencies:  <bool>   <stdlib>   <stdint>   <string.h>        */
/*  Utility headers from the OEIS build system.                         */
/* ------------------------------------------------------------------ */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/build_spf.h"
#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------------ */
/*  Static state                                                   */
/* ------------------------------------------------------------------ */

static seq_t *cache = NULL;          /* stored terms a(1), a(2), ...          */
static size_t cache_len = 0;         /* number of cached terms               */
static size_t cache_cap = 0;         /* allocated capacity of cache           */

static idx_t *spf = NULL;            /* smallest prime factor array           */
static idx_t *primes = NULL;         /* array of primes up to limit           */
static size_t prime_cnt = 0;          /* number of primes found                */
static idx_t *prime_index = NULL;    /* prime -> 1‑based index (size = limit+1) */
static idx_t sieve_limit = 0;        /* current sieve limit                  */

static int *seen = NULL;             /* timestamped seen array for divisors   */
static size_t seen_cap = 0;          /* capacity of seen[]                   */
static int cur_mark = 1;             /* current timestamp                    */

/* ------------------------------------------------------------------ */
/*  Utility helpers                                                 */
/* ------------------------------------------------------------------ */

static void ensure_seen_cap(size_t sz)
{
    if (sz <= seen_cap) return;
    int *tmp = (int*)realloc(seen, sz * sizeof(int));
    if (!tmp) exit(EXIT_FAILURE);
    seen = tmp;
    memset(seen, 0, sz * sizeof(int));
    seen_cap = sz;
}

static void rebuild_tables(idx_t new_lim)
{
    free(spf);
    free(primes);
    free(prime_index);

    build_spf(new_lim, &spf);
    build_primes_upto(new_lim, &primes, &prime_cnt);

    prime_index = (idx_t*)calloc((size_t)new_lim + 1, sizeof(idx_t));
    if (!prime_index) exit(EXIT_FAILURE);
    for (idx_t i = 0; i < prime_cnt; ++i)
        prime_index[primes[i]] = i + 1;   /* 1‑based index */

    sieve_limit = new_lim;
}

static void ensure_limit(idx_t x)
{
    if (x <= sieve_limit) return;
    idx_t lim = sieve_limit ? sieve_limit << 1 : 32ULL;
    while (lim < x) lim <<= 1;
    rebuild_tables(lim);
}

static seq_t divisor_count(idx_t n)
{
    seq_t d = 1;
    idx_t m = n;
    while (m > 1) {
        idx_t p = spf[m];
        size_t cnt = 0;
        while (m % p == 0) {
            m /= p;
            ++cnt;
        }
        d *= (cnt + 1);
    }
    return d;
}

static size_t distinct_primes(idx_t n, idx_t *list, size_t maxsz)
{
    size_t np = 0;
    idx_t m = n;
    while (m > 1) {
        idx_t p = spf[m];
        while (m % p == 0) m /= p;
        if (np < maxsz) list[np++] = p;
    }
    return np;
}

static seq_t distinct_index_divisors(idx_t n)
{
    if (n == 1) return 0;   /* empty set */

    /* Ensure storage for potential divisor indices */
    ensure_seen_cap(prime_cnt + 1);

    static idx_t prime_list[64];
    size_t np = distinct_primes(n, prime_list, sizeof(prime_list)/sizeof(prime_list[0]));

    int mark = cur_mark++;
    if (cur_mark == 0) {    /* overflow of timestamp */
        memset(seen, 0, seen_cap * sizeof(int));
        cur_mark = 1;
        mark = cur_mark;
    }

    int uniq = 0;
    for (size_t i = 0; i < np; ++i) {
        idx_t idx = prime_index[prime_list[i]];   /* 1‑based index of this prime */

        for (idx_t d = 1; d * d <= idx; ++d) {
            if (idx % d == 0) {
                if (seen[(size_t)d] != mark) {
                    seen[(size_t)d] = mark;
                    ++uniq;
                }
                idx_t e = idx / d;
                if (e != d && seen[(size_t)e] != mark) {
                    seen[(size_t)e] = mark;
                    ++uniq;
                }
            }
        }
    }
    return (seq_t)uniq;
}

/* ------------------------------------------------------------------ */
/*  Generation routine                                             */
/* ------------------------------------------------------------------ */

static void generate_up_to(size_t target)
{
    static idx_t candidate = 1;               /* next integer to test */
    while (cache_len < target) {
        ensure_limit(candidate);

        seq_t dcnt = divisor_count(candidate);
        seq_t uniq = distinct_index_divisors(candidate);

        if (dcnt > uniq) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
}

/* ------------------------------------------------------------------ */
/*  Public entry point                                              */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* convention for undefined index */
    generate_up_to((size_t)n);
    return cache[n - 1];
}
