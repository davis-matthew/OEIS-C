/*
 * A350373  Numbers with exactly 6 semiprime divisors.
 * https://oeis.org/A350373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   alloc_seq_array, build_spf, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers with exactly 6 semiprime divisors.
 * The routine checks successive integers, counts their semiprime
 * divisors using a sieve of smallest prime factors, and stores each
 * qualifying number in a cache so that subsequent calls are O(1). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Cache for sequence values. */
static seq_t *seq_cache = NULL;  /* array of seq_t values          */
static size_t seq_cap   = 0;     /* allocated capacity             */
static size_t seq_len   = 0;     /* number of elements computed    */

/* Smallest‑prime‑factor sieve. */
static idx_t *spf_cache = NULL; /* array of idx_t                 */
static idx_t  spf_limit = 0;    /* upper bound of current sieve   */

/* Next integer to be examined. */
static idx_t current_m = 2;

/* Append a new value to the cache, growing the array when necessary. */
static void append_value(seq_t val)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 128;
        seq_t *new_arr = alloc_seq_array(new_cap);
        if (seq_cache) {
            for (size_t i = 0; i < seq_len; ++i)
                new_arr[i] = seq_cache[i];
            free_seq_array(seq_cache);
        }
        seq_cache = new_arr;
        seq_cap   = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, return 0 for safety */
        return (seq_t)0;

    /* Quick return if already computed. */
    if ((idx_t)seq_len >= n)
        return seq_cache[n - 1];

    /* Generate new terms until we have at least n values. */
    while (seq_len < (size_t)n) {
        /* Ensure the SPF sieve covers the number being tested. */
        if (current_m > spf_limit) {
            idx_t new_limit = spf_limit ? spf_limit * 2 : 1024;
            if (new_limit < current_m)
                new_limit = current_m * 2;
            free(spf_cache);
            build_spf(new_limit, &spf_cache);
            spf_limit = new_limit;
        }

        /* Factor current_m using the SPF table. */
        idx_t temp = current_m;
        size_t k = 0;   /* distinct prime factors          */
        size_t r = 0;   /* primes with exponent ≥ 2        */
        while (temp > 1) {
            idx_t p = spf_cache[temp];
            size_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            ++k;
            if (cnt >= 2)
                ++r;
        }

        /* Count semiprime divisors: r + C(k,2). */
        idx_t comb = (idx_t)(k * (k - 1) / 2);
        idx_t semiprime_cnt = r + comb;
        if (semiprime_cnt == 6)
            append_value((seq_t)current_m);

        ++current_m;
    }

    return seq_cache[n - 1];
}
