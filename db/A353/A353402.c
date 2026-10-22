/*
 * A353402  Numbers k such that the k-th composition in standard order has its
 *          own run-lengths as a subsequence (not necessarily consecutive).
 * https://oeis.org/A353402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A353402: a(n) = the n-th nonnegative integer k such that
 * the k‑th composition in standard reverse‑lexicographic order has its
 * own run‑lengths as a subsequence (not necessarily consecutive) of the
 * composition itself.
 * The function builds the sequence on demand, caching computed values
 * across calls.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for already found terms */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    /* next k to be tested for the property */
    static idx_t next_k = 0;

    /* extend cache until it contains at least n+1 entries */
    while (cache_size <= (size_t)n) {
        idx_t k = next_k++;
        /* Build composition of k */

        unsigned int diffs[65];      /* differences of positions of 1 bits */
        size_t diff_len = 0;
        unsigned int prev = 0;

        idx_t x = k;
        unsigned int pos = 1;        /* bit position (1‑based from LSB) */
        while (x > 0) {
            if (x & 1ULL) {
                unsigned int cur = pos;
                diffs[diff_len++] = cur - prev;
                prev = cur;
            }
            x >>= 1;
            pos++;
        }

        /* The composition is the reversed differences vector */
        unsigned int comp[65];
        size_t comp_len = diff_len;
        for (size_t i = 0; i < comp_len; ++i)
            comp[i] = diffs[diff_len - 1 - i];

        /* Compute run‑lengths of the composition */
        unsigned int runs[65];
        size_t run_len = 0;
        size_t run_arr_len = 0;
        for (size_t i = 0; i < comp_len; ) {
            run_len = 1;
            while (i + run_len < comp_len && comp[i + run_len] == comp[i])
                ++run_len;
            runs[run_arr_len++] = (unsigned int)run_len;
            i += run_len;
        }

        /* Check whether runs is a subsequence of comp */
        bool ok = true;
        size_t p = 0;          /* current index in comp */
        for (size_t r = 0; r < run_arr_len; ++r) {
            unsigned int need = runs[r];
            while (p < comp_len && comp[p] != need)
                ++p;
            if (p == comp_len) {      /* no further match */
                ok = false;
                break;
            }
            ++p;                      /* move past matched element */
        }

        if (ok) {
            /* Ensure cache space */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_buf) abort(); /* allocation failure */
                cache = new_buf;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)k;
        }
    }

    return cache[(size_t)n];
}
