/*
 * A345405  Integers k such that k = (d1)_c + (d2)_c + ... + (dc)_c, where
 *          (d)_c denotes the descending factorial of d, c is the length of k
 *          and di is the i-th digit of k in base 10.
 * https://oeis.org/A345405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A345405: integers k such that
   k = Σ_{i=1..c} (d_i)_c
   where c = number of decimal digits of k and
   (d)_c = d·(d-1)·…·(d-c+1).  Computed by exhaustive search. */

#include <stdlib.h>
#include <stdbool.h>

/* static cache of generated terms, stored in increasing order. */
static seq_t *cache   = NULL;   /* pointer to array of terms           */
static size_t cache_len = 0;    /* number of terms currently cached    */
static size_t cache_cap = 0;    /* allocated capacity of cache         */
static seq_t next_candidate = 0;

/* store an additional term in the cache, enlarging it if needed. */
static void push_candidate(seq_t val) {
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 64;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* test whether k satisfies the defining property. */
static bool is_valid(seq_t k) {
    /* extract decimal digits; most significant digit first (the order does not
       matter for the sum).  The array holds up to 20 digits: enough for any
       64‑bit integer and arbitrary precision values below 10^20. */
    int digits[20];
    size_t len = 0;

    if (k == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        seq_t tmp = k;
        /* collect digits in reverse (least significant first). */
        while (tmp != 0) {
            digits[len++] = (int)(tmp % 10);
            tmp /= 10;
        }
    }

    /* compute the sum of descending factorials of the digits. */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        int d = digits[i];
        if (d < (int)len)   /* (d)_len = 0 when d < len */
            continue;
        seq_t prod = 1;
        for (size_t t = 0; t < len; ++t)
            prod *= (seq_t)(d - (int)t);
        sum += prod;
    }

    return sum == k;
}

/* Main function requested by the build. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* under‑offset, harmless */

    /* lazy initialise the cache once.  We always preload the first ten
       terms (0 to 9) because they are trivially valid. */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (seq_t val = 0; val <= 9; ++val)   /* always insert 0–9 */
            push_candidate(val);
        next_candidate = 10;
    }

    /* generate further terms until the nth term is available. */
    while (cache_len < (size_t)n) {
        if (is_valid(next_candidate))
            push_candidate(next_candidate);
        ++next_candidate;
    }

    return cache[n - 1];
}
