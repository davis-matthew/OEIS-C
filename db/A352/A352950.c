/*
 * A352950  Lexicographically earliest infinite sequence of distinct
 *          nonnegative integers commencing 1,3,5,7 such that any four
 *          consecutive terms are pairwise coprime.
 * https://oeis.org/A352950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   enlarge_used, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence of distinct nonnegative integers
   such that any four consecutive terms are pairwise coprime.
   The first terms are 1,3,5,7.  A(n) is computed for all n ≥ 1. */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/enlarge_used.h"

seq_t a(idx_t n)
{
    /* Static state that is initialised once. */
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static bool *used = NULL;
    static size_t used_cap = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Initial allocation for the sequence. */
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) abort();

        /* Initial allocation for the used‑array. */
        enlarge_used(&used_cap, &used, 16);

        /* First four terms. */
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)3;
        seq[2] = (seq_t)5;
        seq[3] = (seq_t)7;
        seq_len = 4;

        /* Mark these values as used. */
        used[(size_t)1] = true;
        used[(size_t)3] = true;
        used[(size_t)5] = true;
        used[(size_t)7] = true;

        initialized = true;
    }

    if (n == 0) return 0;               /* undefined for n < 1 */

    /* Grow the sequence until we have a( n ). */
    while (seq_len < (size_t)n) {
        seq_t cand = 0;                    /* start from 0 */
        for (;;) {
            /* Ensure the used‑array can index cand. */
            size_t idx = (size_t)cand;
            if (idx >= used_cap) enlarge_used(&used_cap, &used, idx + 1);

            if (used[idx]) {                  /* already used? skip */
                ++cand;
                continue;
            }

            /* Check coprimality with the previous three terms. */
            bool ok = true;
            if (seq_len >= 1 && gcd(cand, seq[seq_len-1]) != (seq_t)1) ok = false;
            if (ok   && seq_len >= 2 && gcd(cand, seq[seq_len-2]) != (seq_t)1) ok = false;
            if (ok   && seq_len >= 3 && gcd(cand, seq[seq_len-3]) != (seq_t)1) ok = false;

            if (ok) break;                    /* found the smallest candidate */
            ++cand;
        }

        /* Append the candidate to the sequence. */
        if (seq_len >= seq_cap) {              /* grow the sequence array */
            seq_cap <<= 1;
            seq_t *new_seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            if (!new_seq) abort();
            seq = new_seq;
        }
        seq[seq_len++] = cand;

        /* Mark the value as used. */
        size_t idx = (size_t)cand;
        if (idx >= used_cap) enlarge_used(&used_cap, &used, idx + 1);
        used[idx] = true;
    }

    return seq[n-1];
}
