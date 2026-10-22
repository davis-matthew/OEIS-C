/*
 * A360252  Numbers for which the prime indices have greater mean than the
 *          distinct prime indices.
 * https://oeis.org/A360252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer whose average prime index
 *            (counted with multiplicity) exceeds the average of its distinct
 *            prime indices
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a linear sieve for smallest prime factors
   and their indices, then scans integers until enough qualifying terms
   have been found, caching them.  It works for integer seq_t (8‑128‑bit)
   and arbitrary‑precision seq_t (via seq_alloc/seq_realloc). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_realloc.h"   /* for seq_realloc(), seq_alloc_new_array() */

static seq_t *result_cache = NULL;
static size_t res_len = 0, res_cap = 0;

static uint32_t *spf = NULL;           /* smallest prime factor (as uint32_t) */
static uint32_t *prime_index = NULL;   /* 1‑based index of a prime value */
static size_t spf_cap = 0;             /* maximum value currently sieved */
static uint64_t last_checked = 1;      /* last integer tested */

/* Ensure sieve data is computed up to at least ‘need’ (inclusive). */
static void
ensure_sieve(size_t need)
{
    if (need <= spf_cap) return;

    size_t new_cap = spf_cap ? spf_cap * 2 : 8;
    if (new_cap < need) new_cap = need;

    uint32_t *new_spf = (uint32_t *)malloc((new_cap + 1) * sizeof(uint32_t));
    uint32_t *new_prime_index = (uint32_t *)malloc((new_cap + 1) * sizeof(uint32_t));

    // Initialise spf
    for (size_t i = 2; i <= new_cap; ++i) new_spf[i] = (uint32_t)i;

    uint32_t idx = 0;
    for (size_t i = 2; i <= new_cap; ++i) {
        if (new_spf[i] == (uint32_t)i) {              /* i is prime */
            ++idx;
            new_prime_index[i] = idx;
            if (i * i <= new_cap) {
                for (size_t j = i * i; j <= new_cap; j += i) {
                    if (new_spf[j] == (uint32_t)j) new_spf[j] = (uint32_t)i;
                }
            }
        }
    }

    free(spf);
    free(prime_index);
    spf = new_spf;
    prime_index = new_prime_index;
    spf_cap = new_cap;
}

/* Ensure result cache can handle at least ‘need’ elements. */
static void
ensure_result_capacity(size_t need)
{
    if (need <= res_cap) return;
    size_t new_cap = res_cap ? res_cap * 2 : 8;
    if (new_cap < need) new_cap = need;

    seq_t *new_arr = seq_realloc(result_cache, res_cap, new_cap);
    result_cache = new_arr;
    res_cap = new_cap;
}

/* Test whether an integer satisfies the mean‑index condition. */
static bool
qualifies(uint64_t val)
{
    uint64_t tmp = val;
    seq_t sum_mult = 0, sum_dist = 0;
    seq_t cnt_mult = 0, cnt_dist = 0;

    while (tmp > 1) {
        uint32_t p = spf[tmp];
        uint32_t idx = prime_index[p];
        seq_t mult = 0;

        do {
            tmp /= p;
            ++mult;
            sum_mult += (seq_t)idx;
        } while (tmp > 1 && spf[tmp] == p);

        sum_dist += (seq_t)idx;
        ++cnt_dist;
        cnt_mult += mult;
    }

    /* (sum_mult / cnt_mult) > (sum_dist / cnt_dist)  <=>  sum_mult*cnt_dist > sum_dist*cnt_mult */
    return (sum_mult * cnt_dist) > (sum_dist * cnt_mult);
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;

    /* Initialise cache only once */
    if (!result_cache) {
        result_cache = NULL;
        res_len = res_cap = 0;
        spf = NULL;
        prime_index = NULL;
        spf_cap = 0;
        last_checked = 1;
    }

    /* Generate qualifying terms until we have at least n of them */
    while (res_len < n) {
        ++last_checked;
        if ((uint64_t)last_checked > spf_cap) {
            ensure_sieve((size_t)last_checked);
        }
        if (qualifies(last_checked)) {
            ensure_result_capacity(res_len + 1);
            result_cache[res_len++] = (seq_t)last_checked;
        }
    }

    return result_cache[n - 1];
}
