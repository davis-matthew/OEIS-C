/*
 * A345585  Numbers that are the sum of eight fourth powers in ten or more
 *          ways.
 * https://oeis.org/A345585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2273, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A345585: Numbers that are the sum of eight fourth powers
 * in ten or more ways.
 * 
 * The value is obtained by searching all sums that can be formed by
 * exactly eight (unordered) fourth powers and counting how many
 * distinct representations exist.  All sums with at least ten
 * representations are stored in increasing order; the returned
 * value a(n) is the n‑th element of that list (with a(1) the first
 * term shown in the OEIS entry).
 * 
 * The implementation uses a bounded 3‑dimensional dynamic programming
 * table dp[j][s] = number of representations of the sum s using
 * exactly j fourth powers, capped at 10.  For each power value v = k^4
 * the table is updated in ascending order of v.  When a new
 * representation count reaches ten we record the sum.  
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* global cache */
static seq_t *seq_vals   = NULL;  /* array of found sums */
static size_t   seq_len  = 0;     /* number of stored sums */
static size_t   seq_cap  = 0;     /* allocated capacity */
static idx_t   current_max_sum = 0;   /* upper bound used in last dp run */

/* rebuild the cache up to a new maximum sum */
static void rebuild_cache(idx_t max_sum) {
    /* free old cache */
    free(seq_vals);
    seq_vals = NULL;
    seq_len  = 0;
    seq_cap  = 0;

    /* DP table dimensions: 9 rows (0..8 parts), columns 0..max_sum */
    size_t M      = (size_t)max_sum + 1;
    size_t rows   = 9;
    size_t total  = rows * M;
    uint8_t *dp   = (uint8_t *)calloc(total, sizeof(uint8_t));
    if (!dp) return;            /* out‑of‑memory – caller will not crash */

    /* dp[0][0] = 1 */
    dp[0] = 1;

    /* enumerate fourth powers <= max_sum */
    for (uint64_t k = 1; ; ++k) {
        uint64_t v64 = k * k * k * k;
        if (v64 > max_sum) break;
        size_t v = (size_t)v64;

        /* update DP for each part count */
        for (int j = 1; j <= 8; ++j) {
            size_t jrow    = j * M;
            size_t prevrow = (j - 1) * M;

            for (size_t s = v; s < M; ++s) {
                uint8_t add = dp[prevrow + (s - v)];
                if (!add) continue;
                uint8_t cur = dp[jrow + s];
                int newv = cur + add;
                if (newv > 10) newv = 10;
                dp[jrow + s] = (uint8_t)newv;
            }
        }
    }

    /* collect sums with at least 10 representations */
    for (size_t s = 1; s < M; ++s) { /* skip 0 */
        if (dp[8 * M + s] >= 10) {
            if (seq_len >= seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 128;
                seq_t *newp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
                if (!newp) break;          /* allocation failure, abort */
                seq_vals = newp;
                seq_cap  = new_cap;
            }
            seq_vals[seq_len++] = (seq_t)s;
        }
    }

    free(dp);
}

/* public function */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* not part of the sequence */

    /* compute up to requested index if needed */
    if (seq_len < (size_t)n) {
        /* start with a modest bound and double until enough terms are found */
        if (current_max_sum == 0) current_max_sum = 200000; /* initial guess */
        while (seq_len < (size_t)n) {
            rebuild_cache(current_max_sum);
            if (seq_len >= (size_t)n) break;
            current_max_sum <<= 1;       /* double the search bound */
        }
    }
    return seq_vals[(size_t)n - 1];
}
