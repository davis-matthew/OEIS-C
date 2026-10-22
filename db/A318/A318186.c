/*
 * A318186  Totally transitive numbers. Matula-Goebel numbers of totally
 *          transitive rooted trees.
 * https://oeis.org/A318186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   contains_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Totally Transitive Numbers (OEIS A318186)
 * The function generates the sequence by testing each integer for the
 * definition of a totally transitive number.  No closed formula is known,
 * so the implementation performs a search with memoisation of the
 * transitivity status of indices.  All large integer handling is done
 * with the supplied type seq_t.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains_value.h"

/* -------------------------------------------------------------------- */
/* Parameters and global storage                                       */
/* -------------------------------------------------------------------- */
#define SIEVE_LIMIT 2000000          /* should cover all primes up to 10000th term */

static int  *spf          = NULL;   /* smallest prime factor for each number */
static int  *prime_index  = NULL;   /* index of a prime, 0 if composite */
static uint8_t *index_state = NULL; /* 0=unknown, 1=false, 2=true */

static seq_t  *cache      = NULL;   /* 0‑based cache: cache[0] = a(1) */
static size_t  cache_cap  = 0;
static size_t  cache_len  = 0;      /* number of cached terms */
static idx_t   last_scan  = 0;      /* last number tested during generation */

/* -------------------------------------------------------------------- */
/* Utility: membership test in an array of idx_t                       */

/* -------------------------------------------------------------------- */
/* Sieve to initialise spf[] and prime_index[]                         */
/* -------------------------------------------------------------------- */
static void init_sieve(void)
{
    size_t i, j;
    size_t limit = SIEVE_LIMIT;
    spf = (int*)calloc(limit + 1, sizeof(int));
    prime_index = (int*)calloc(limit + 1, sizeof(int));

    int primes_count = 0;
    for (i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                     /* i is prime */
            spf[i] = (int)i;
            prime_index[i] = ++primes_count;
            /* mark multiples */
            if (i <= limit / i) {
                for (j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = (int)i;
            }
        }
    }

    /* allocate memoisation table for indices (indices run 1..primes_count) */
    index_state = (uint8_t*)calloc((size_t)(primes_count + 1), sizeof(uint8_t));
    /* 1 corresponds to the integer 1 in the index space */
    index_state[1] = 2;   /* 1 is totally transitive */
}

/* -------------------------------------------------------------------- */
/* Recursive check whether an index (i.e., the index of a prime) is
 * totally transitive.  index 1 is the base case (true).                 */
/* -------------------------------------------------------------------- */
static bool index_is_tot_trans(idx_t idx)
{
    if (idx == 1)
        return true;

    /* if already known, use memoised result */
    if (index_state[idx] != 0)
        return index_state[idx] == 2;

    /* assume false until proven true */
    index_state[idx] = 1;

    /* collect distinct prime factors of idx */
    idx_t factors[64];
    int   fcnt = 0;
    idx_t tmp = idx;
    idx_t last = 0;
    while (tmp > 1) {
        int p = spf[tmp];
        idx_t val = (idx_t)p;
        if (val != last) {
            factors[fcnt++] = val;
            last = val;
        }
        tmp /= p;
        while (tmp % p == 0) tmp /= p;
    }

    /* Requirement (1): all prime indices must themselves be totally
       transitive. */
    for (int k = 0; k < fcnt; ++k) {
        int y = prime_index[(int)factors[k]];
        if (y == 0) {                     /* should not happen */
            index_state[idx] = 1;
            return false;
        }
        if (!index_is_tot_trans((idx_t)y)) {
            index_state[idx] = 1;
            return false;
        }
    }

    /* Requirement (2): closure under prime factors of indices. */
    for (int k = 0; k < fcnt; ++k) {
        int y = prime_index[(int)factors[k]];   /* index of the prime */
        idx_t temp = (idx_t)y;
        idx_t lastq = 0;
        while (temp > 1) {
            int q = spf[temp];
            idx_t valq = (idx_t)q;
            if (valq != lastq) {
                /* valq must also be a prime factor of the original idx
                   (i.e., of the number this index refers to). */
                if (!contains_value(factors, fcnt, valq)) {
                    index_state[idx] = 1;
                    return false;
                }
                lastq = valq;
            }
            temp /= q;
            while (temp % q == 0) temp /= q;
        }
    }

    index_state[idx] = 2;   /* mark as true */
    return true;
}

/* -------------------------------------------------------------------- */
/* Test whether a candidate integer is totally transitive.               */
/* -------------------------------------------------------------------- */
static bool number_is_tot_trans(idx_t num)
{
    if (num == 1)
        return true;

    /* collect distinct prime factors of num */
    idx_t pf[64];
    int pf_cnt = 0;
    idx_t tmp = num;
    idx_t last = 0;
    while (tmp > 1) {
        int p = spf[tmp];
        idx_t val = (idx_t)p;
        if (val != last) {
            pf[pf_cnt++] = val;
            last = val;
        }
        tmp /= p;
        while (tmp % p == 0) tmp /= p;
    }

    /* Requirement (1): each prime factor's index must be totally transitive */
    for (int i = 0; i < pf_cnt; ++i) {
        int y = prime_index[(int)pf[i]];
        if (y == 0) /* shouldn't happen */
            return false;
        if (!index_is_tot_trans((idx_t)y))
            return false;
    }

    /* Requirement (2): for each prime factor p, all prime factors of its
       index must also divide num (i.e., appear among pf). */
    for (int i = 0; i < pf_cnt; ++i) {
        int y = prime_index[(int)pf[i]];
        idx_t temp = (idx_t)y;
        idx_t lastq = 0;
        while (temp > 1) {
            int q = spf[temp];
            idx_t valq = (idx_t)q;
            if (valq != lastq) {
                if (!contains_value(pf, pf_cnt, valq)) {
                    return false;
                }
                lastq = valq;
            }
            temp /= q;
            while (temp % q == 0) temp /= q;
        }
    }

    return true;
}

/* -------------------------------------------------------------------- */
/* Ensure the cache contains at least n terms.                          */
/* -------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* scan for next tot trans number */
        idx_t cand = last_scan + 1;
        for (;; ++cand) {
            if (number_is_tot_trans(cand)) {
                /* store in cache */
                if (cache_len + 1 > cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                    cache_cap = new_cap;
                }
                cache[cache_len] = (seq_t)cand;
                ++cache_len;
                last_scan = cand;
                break;
            }
        }
    }
}

/* -------------------------------------------------------------------- */
/* Public sequence function.                                            */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool first = true;
    if (first) {
        first = false;
        init_sieve();
    }
    ensure_cache(n);
    return cache[n - 1];
}
