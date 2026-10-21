/*
 * A261220  Ranks of involutions in permutation orderings A060117 and A060118.
 * https://oeis.org/A261220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 995, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A261220: numbers whose factorial base representation has no
    pair of non-zero digits d_i and d_j such that either
      (i - d_i) = j   or   (j - d_j) = i   or   (i - d_i) = (j - d_j).
    Index a(0) = 0.  The function returns the nth term for any n >= 0.
    The implementation caches computed terms and tests each integer using
    the described factorial-base condition.  All operations obey the
    restrictions on types and operators. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper: check whether a value satisfies the A261220 condition. */
static bool is_valid(seq_t val)
{
    seq_t remaining = val;
    idx_t pos = 1;                         /* factorial position (1‑based) */
    idx_t nonzero_cnt = 0;                 /* number of non‑zero digits found */

    /* Storage for positions and digits of non‑zero coefficients. */
    /* 64 digits is enough for all values considered in the tests. */
    const size_t MAX_DIGITS = 64;
    idx_t pos_array[MAX_DIGITS];
    seq_t digit_array[MAX_DIGITS];

    while (remaining > 0) {
        seq_t base = (seq_t)(pos + 1);     /* next factorial base (i+1)     */
        seq_t d = remaining % base;       /* digit in this position        */
        if (d != 0) {
            pos_array[nonzero_cnt] = pos;
            digit_array[nonzero_cnt] = d;
            nonzero_cnt++;
        }
        remaining /= base;
        pos++;
    }

    /* Test all unordered pairs of non‑zero digits. */
    for (idx_t a = 0; a < nonzero_cnt; ++a) {
        idx_t i = pos_array[a];
        seq_t di = digit_array[a];
        idx_t left_i = i - (idx_t)di;     /* i - d_i                      */
        for (idx_t b = a + 1; b < nonzero_cnt; ++b) {
            idx_t j = pos_array[b];
            seq_t dj = digit_array[b];
            idx_t left_j = j - (idx_t)dj; /* j - d_j                      */

            /* Condition: no pair satisfies i-d_i = j or j-d_j = i
               or i-d_i = j-d_j. */
            if (left_i == j || left_j == i || left_i == left_j)
                return false;
        }
    }
    return true;
}

/* Cache for already computed terms.  The array grows as more terms
   are required. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0; /* allocated capacity          */
static size_t cache_len  = 0; /* number of terms stored      */
static seq_t next_val     = 0; /* smallest value not yet tested */

/* Increase cache size to hold at least 'need' terms. */
static void enlarge_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap * 2;
    if (new_cap < need) new_cap = need;
    if (new_cap == 0) new_cap = 16;           /* initial capacity         */
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    for (size_t i = 0; i < cache_len; ++i) new_arr[i] = cache[i];
    free(cache);
    cache = new_arr;
    cache_cap = new_cap;
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n < 0) return 0; /* not defined, keep type safety */

    /* Fill cache up to index n. */
    while (cache_len <= (size_t)n) {
        if (is_valid(next_val)) {
            enlarge_cache(cache_len + 1);
            cache[cache_len] = next_val;
            cache_len++;
        }
        next_val = next_val + 1;
    }
    return cache[(size_t)n];
}
