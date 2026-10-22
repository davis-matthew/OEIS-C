/*
 * A343196  a(n) is the least positive number k such that Sum_{k<=j<=k+n-1}
 *          A001414(j) is prime.
 * https://oeis.org/A343196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 790, u32 n <= 127, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = the least k such that
   Sum_{j=k}^{k+n-1} A001414(j) is prime.
   The function computes the sum of prime‑factor sums A001414(j)
   using a sieve of smallest prime factors and searches for the
   first prime sum in a sliding window.  It caches each result
   so later calls for the same n are O(1). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* ---------- Static data structures ------------------------------------ */

static seq_t *cache = NULL;          /* cache of computed a(n) values       */
static size_t cache_cap = 0;         /* current cache capacity in entries   */

static uint32_t *spf_arr = NULL;     /* smallest prime factor array         */
static seq_t   *s_arr   = NULL;     /* A001414(i) array                    */
static size_t   spf_limit = 0;       /* number of elements in the above arrays */

/* ---------- Helper to build sieve and sum array ----------------------- */
static void build_sieve(size_t limit)
{
    uint32_t *new_spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    seq_t    *new_s   = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!new_spf || !new_s) return;  /* in case of OOM we simply return */

    memset(new_spf, 0, (limit + 1) * sizeof(uint32_t));
    new_spf[0] = 0;
    new_spf[1] = 1;

    for (size_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= (uint64_t)limit) {
                for (size_t j = i * i; j <= limit; j += i) {
                    if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
                }
            }
        }
    }

    new_s[0] = 0;
    new_s[1] = 0;
    for (size_t i = 2; i <= limit; ++i) {
        /* A001414(i) = A001414(i / spf[i]) + spf[i] */
        new_s[i] = new_s[i / new_spf[i]] + (seq_t)new_spf[i];
    }

    free(spf_arr);
    free(s_arr);
    spf_arr   = new_spf;
    s_arr     = new_s;
    spf_limit = limit + 1;           /* indices 0..limit are valid           */
}

/* ---------- Main sequence function ------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure global structures are initialized */
    static bool initialized = false;
    if (!initialized) {
        /* A generous initial size; can be increased later if needed */
        size_t init_limit = 2000000;
        if ((size_t)n > 100000) init_limit = (size_t)n * 20 + 100;
        build_sieve(init_limit);
        initialized = true;
    }

    /* Expand cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;                 /* OOM guard */
        /* initialize newly allocated entries to 0 (uncomputed) */
        memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache     = new_cache;
        cache_cap = new_cap;
    }
    if (cache[n] != 0) return cache[n];          /* already computed */

    /* Main search for the minimal k */
    idx_t  k   = 1;
    seq_t  sum = 0;

    /* Initial sum for window starting at k=1 */
    for (idx_t j = 1; j <= n; ++j) {
        if ((size_t)j >= spf_limit) {
            /* Extend the sieve if required (very unlikely for the target limits) */
            size_t new_limit = ((size_t)j + 1) * 2 + 100;
            build_sieve(new_limit);
        }
        sum += s_arr[(size_t)j];
    }

    while (true) {
        /* Check primality of current window sum */
        if (is_prime_big(sum)) {
            cache[n] = k;
            return k;
        }

        /* Prepare for next k */
        ++k;
        idx_t out_index = k - 1;            /* leaving value */
        idx_t in_index  = k + n - 1;        /* entering value */

        if ((size_t)in_index >= spf_limit) {
            /* Extend the sieve if necessary */
            size_t new_limit = ((size_t)in_index + 1) * 2 + 100;
            build_sieve(new_limit);
        }

        sum -= s_arr[(size_t)out_index];
        sum += s_arr[(size_t)in_index];
    }
}
