/*
 * A262411  Lexicographically earliest sequence of distinct terms such that
 *          the ternary representations of two consecutive terms overlap.
 * https://oeis.org/A262411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   lexicographically earliest permutation of natural numbers * such
 *            that consecutive ternary representations overlap. * Computed by
 *            a greedy search
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_ternary, has_overlap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_ternary.h"
#include "../utils/has_overlap.h"

/* Global state for lazy generation */
static seq_t *perm = NULL;          /* array of generated terms                   */
static idx_t  perm_cap = 0;         /* allocated capacity of perm                 */
static idx_t  perm_len = 0;         /* number of terms already generated          */

static bool *used = NULL;           /* used[x] == true if x already appeared      */
static idx_t  used_cap = 0;         /* allocated capacity of used array           */

static seq_t  prev_val = 0;         /* a(k-1)                                    */

/* Ensure used array can index up to idx (inclusive).  */
static void ensure_used_capacity(idx_t idx)
{
    if (idx < used_cap) return;
    idx_t new_cap = used_cap ? used_cap * 2 : 8;
    while (new_cap <= idx) new_cap *= 2;
    bool *new_used = (bool*)realloc(used, new_cap * sizeof(bool));
    if (!new_used) abort();               /* memory exhausted */
    /* initialize newly allocated slots to false */
    for (idx_t i = used_cap; i < new_cap; ++i) new_used[i] = false;
    used = new_used;
    used_cap = new_cap;
}

/* Ensure perm array can hold at least length elements.  */
static void ensure_perm_capacity(idx_t length)
{
    if (length <= perm_cap) return;
    idx_t new_cap = perm_cap ? perm_cap * 2 : 8;
    while (new_cap < length) new_cap *= 2;
    seq_t *new_perm = (seq_t*)realloc(perm, new_cap * sizeof(seq_t));
    if (!new_perm) abort();
    perm = new_perm;
    perm_cap = new_cap;
}

/* Generate the permutation up to index n (inclusive of n). */
static void generate_up_to(idx_t n)
{
    if (perm_len >= n) return;               /* already generated */

    /* First term initialization. */
    if (perm_len == 0) {
        ensure_perm_capacity(1);
        perm[0] = 1;
        perm_len = 1;
        prev_val = 1;

        ensure_used_capacity(1);
        used[1] = true;
    }

    /* Generate further terms until we reach length n. */
    while (perm_len < n) {
        /* Find the smallest unused candidate overlapping prev_val. */
        idx_t cand = 1;
        for (;;) {
            ensure_used_capacity(cand);
            if (used[cand]) {
                ++cand;
                continue;
            }
            if (has_overlap(prev_val, (seq_t)cand)) {
                break;                 /* suitable candidate */
            }
            ++cand;
        }

        seq_t val = (seq_t)cand;
        used[cand] = true;

        /* Append to permutation. */
        ensure_perm_capacity(perm_len + 1);
        perm[perm_len] = val;
        prev_val = val;
        ++perm_len;
    }
}

/* External function implementation. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n = 0 */
    generate_up_to(n);
    return perm[n - 1];
}
