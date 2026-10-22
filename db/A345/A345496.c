/*
 * A345496  Numbers that are the sum of eight squares in nine or more ways.
 * https://oeis.org/A345496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 186, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   free_seq_buf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A345496: Numbers that are the sum of eight (positive) squares in nine or
   more (unordered) ways. The function computes terms by dynamic programming
   that counts, for each sum s, the number of unordered representations
   using exactly 8 positive squares. It records s when that number is at
   least 9. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/free_seq_buf.h"

/* Forward declarations for helper functions from the utilities library. */
static void free_seq_buf(seq_t *p);
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val);

/* Dynamic storage for the sequence itself.  seq[i] holds a(i+1). */
static seq_t *seq      = NULL;
static size_t seq_len  = 0;   /* number of elements stored */
static size_t seq_cap  = 0;   /* allocated capacity */
static size_t processed_limit = 0;   /* largest limit for which the table
                                     * has been built. */

/* Append a value to a dynamic seq_t array, resizing when needed. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap > 0) ? (*cap << 1) : 8;
        seq_t *new_arr;
#ifdef __cplusplus
        new_arr = new seq_t[new_cap];
        for (size_t i = 0; i < *len; ++i) new_arr[i] = (*p)[i];
        delete[] *p;
#else
        new_arr = (seq_t*)realloc(*p, new_cap * sizeof(seq_t));
#endif
        *p   = new_arr;
        *cap = new_cap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Rebuild the whole sequence up to 'limit'.  The DP table is recomputed
   and all qualifying numbers ≤ limit are stored in seq. */
static void recompute_sequence(size_t limit)
{
    /* Release old sequence. */
    if (seq) {
        free_seq_buf(seq);
        seq = NULL;
    }
    seq_len = 0;
    seq_cap = 0;

    /* DP table: (limit+1) × 9 entries, stored linearly.  dp[s*9 + k]
       is the number of unordered representations of s using exactly k
       positive squares.  Values are capped at 9. */
    size_t table_size = (limit + 1) * 9;
    unsigned char *dp = (unsigned char*)calloc(table_size, sizeof(unsigned char));
    if (!dp) return;            /* out of memory: give up */

    dp[0] = 1;   /* one way to make 0 with 0 squares */

    /* For each square value v = i^2, add it to the DP. */
    for (int i = 1; (size_t)i * (size_t)i <= limit; ++i) {
        size_t v = (size_t)i * (size_t)i;   /* coin value i² */
        for (size_t s = v; s <= limit; ++s) {
            size_t base = s * 9;
            size_t srcB = (s - v) * 9;
            for (int k = 1; k <= 8; ++k) {
                unsigned char add = dp[srcB + (k - 1)];
                if (!add) continue;
                unsigned int sum = (unsigned int)(dp[base + k]) + (unsigned int)add;
                dp[base + k] = (sum > 9) ? 9 : (unsigned char)sum;   /* cap at 9 */
            }
        }
    }

    /* Scan sums: record those with at least 9 unordered representations
       using exactly 8 positive squares. */
    for (size_t s = 1; s <= limit; ++s) {
        size_t base = s * 9;
        if (dp[base + 8] >= 9) {
            append_candidate(&seq, &seq_len, &seq_cap, (seq_t)s);
        }
    }

    free(dp);
    processed_limit = limit;
}

/* Public entry point: returns a(n), n ≥ 1.  Calls rebuild logic as
   necessary to obtain enough terms. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, return safe value */

    /* Expand the limit until we have at least n terms. */
    while (seq_len < (size_t)n) {
        size_t new_limit = processed_limit ? processed_limit << 1 : 100;
        if (new_limit <= processed_limit) new_limit = processed_limit + 1; /* safety */
        recompute_sequence(new_limit);
        if (seq_len == 0) break;   /* should not happen */
    }

    /* seq is 0‑based; a(1) is seq[0] */
    return (seq_len >= (size_t)n) ? seq[n - 1] : 0;
}
