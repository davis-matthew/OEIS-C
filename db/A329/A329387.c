/*
 * A329387  Solution sequence (a(n)) of the complementary equation a(n) = 4
 *          b(n+1) - b(n), with b(0) = 1.
 * https://oeis.org/A329387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   extend_used
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complementary sequences: a(n) = 4*b(n+1) - b(n),  b(0)=1.
   The function builds both a and b incrementally and memoises
   values so that any call order is supported. */

/* ---------- required includes ---------- */
#include <stdlib.h>
#include <string.h>
#include "../utils/extend_used.h"

/* ---------- dynamic array helper ---------- */
static void grow_seq(seq_t **array, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*array, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *array = tmp;
        *cap   = new_cap;
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t   *a_vals = NULL;
    static seq_t   *b_vals = NULL;
    static size_t  a_cap  = 0, b_cap = 0;
    static size_t  a_len  = 0, b_len = 0;
    static unsigned char *used = NULL;
    static idx_t      used_size = 0;

    /* initialisation on first call */
    if (a_len == 0) {
        /* b(0) = 1 */
        grow_seq(&b_vals, &b_cap, 1);
        b_vals[0] = (seq_t)1;
        b_len = 1;

        /* allocate bitmap and mark 1 as used (for b) */
        extend_used(&used_size, &used, (idx_t)1);
        used[1] = 1;
    }

    /* compute terms up to n (if necessary) */
    while (a_len <= n) {
        /* current b_k = b(b_len-1) */
        seq_t cur_b = b_vals[b_len - 1];
        idx_t cur_b_idx = (idx_t)cur_b;

        /* search for the smallest b_{k+1} > cur_b that yields
           unused a_k = 4*b_{k+1} - cur_b. */
        idx_t cand_b_idx = cur_b_idx + 1;
        seq_t cand_a = 0;
        for (;;) {
            extend_used(&used_size, &used, cand_b_idx);
            if (used[cand_b_idx]) { ++cand_b_idx; continue; }

            seq_t cand_b = (seq_t)cand_b_idx;
            cand_a = ((seq_t)4 * cand_b) - cur_b;

            idx_t a_idx = (idx_t)cand_a;
            extend_used(&used_size, &used, a_idx);
            if (used[a_idx]) { ++cand_b_idx; continue; }

            /* valid pair found */
            break;
        }

        /* store the new terms */
        grow_seq(&b_vals, &b_cap, b_len + 1);
        b_vals[b_len++] = (seq_t)cand_b_idx;

        grow_seq(&a_vals, &a_cap, a_len + 1);
        a_vals[a_len++] = cand_a;

        /* mark as used */
        used[cand_b_idx] = 1;
        used[(idx_t)cand_a] = 1;
    }

    return a_vals[n];
}
