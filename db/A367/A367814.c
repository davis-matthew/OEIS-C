/*
 * A367814  Lexicographically earliest sequence of distinct nonnegative terms
 *          such that the Levenshtein distance (Ld) between a(n) and a(n+1) is
 *          equal to 4.
 * https://oeis.org/A367814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   ldist4, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the lexicographically earliest sequence of distinct
   nonnegative integers whose successive terms differ by Levenshtein
   distance 4.  The implementation is a greedy search: for each new
   term it scans integer values from 0 upward and selects the first
   unused value that has distance 4 from the previous term. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/seq_to_str.h"   /* seq_to_str(seq_t, char*, size_t*) */
#include "../utils/ldist4.h"

/* --- Cache for the sequence values --- */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static bool  seq_inited = false;

static void seq_ensure(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (seq_cache) {
        memcpy(arr, seq_cache, seq_len * sizeof(seq_t));
        free(seq_cache);
    }
    seq_cache = arr;
    seq_cap   = new_cap;
}

/* Check if a value has already appeared in the sequence */
static bool seq_seen(seq_t v)
{
    for (size_t i = 0; i < seq_len; ++i) {
        if (seq_cache[i] == v) return true;
    }
    return false;
}

/* Compute the next term and append it to seq_cache */
static void compute_next(void)
{
    seq_t prev = seq_cache[seq_len - 1];

    /* Convert previous term to decimal string */
    char prev_buf[256];
    size_t prev_len = 0;
    seq_to_str(prev, prev_buf, &prev_len);

    seq_t cand = 0;
    for (;;) {
        if (!seq_seen(cand)) {
            /* Convert candidate to decimal string */
            char cand_buf[256];
            size_t cand_len = 0;
            seq_to_str(cand, cand_buf, &cand_len);

            if (ldist4(prev_buf, (int)prev_len, cand_buf, (int)cand_len) == 4) {
                seq_ensure(seq_len + 1);
                seq_cache[seq_len++] = cand;
                break;
            }
        }
        ++cand;
    }
}

/* --- Public API --- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* the sequence is defined for n >= 1 */

    if (!seq_inited) {
        seq_inited = true;
        seq_ensure(64);
        seq_cache[0] = 0;   /* a(1) = 0 */
        seq_len = 1;
    }

    while (seq_len < (size_t)n) compute_next();
    return seq_cache[n - 1];
}
