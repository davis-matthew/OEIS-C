/*
 * A363198  a(n) = n for n <= 3; for n >= 4, a(n) is the smallest positive
 *          integer that has not appeared previously in this sequence and
 *          shares a factor with a(n-1) + a(n-2) + a(n-3).
 * https://oeis.org/A363198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A363198 (OEIS)
   a(1) = 1, a(2) = 2, a(3) = 3
   For n ≥ 4, a(n) = the smallest unused positive integer that shares a
   non‑trivial common factor with a(n‑1) + a(n‑2) + a(n‑3). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* --- state for memoisation ------------------------------------------- */
static seq_t *seq        = NULL;   /* array of computed terms (1‑based) */
static idx_t  seq_cap    = 0;     /* allocated capacity of seq       */
static idx_t  seq_len    = 0;     /* number of terms stored          */

static unsigned char *used   = NULL;   /* bitset: 1 = already used     */
static size_t    used_cap  = 0;        /* allocated capacity of used   */

/* Ensure seq has space for at least n+1 elements (index up to n). */
static void
ensure_seq_cap(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (needed > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(*seq));
        seq_cap = new_cap;
    }
}

/* Ensure used can address index n. */
static void
ensure_used_cap(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (needed > used_cap) {
        size_t new_cap = used_cap ? used_cap : 1;
        while (new_cap < needed) new_cap <<= 1;
        unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
        memset(tmp + used_cap, 0, new_cap - used_cap);
        used = tmp;
        used_cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    /* lazy initialisation of the first three terms */
    if (seq == NULL) {
        ensure_seq_cap(3);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        seq[2] = (seq_t)3;
        seq_len = 3;
        /* mark 1,2,3 as used */
        ensure_used_cap(3);
        used[1] = 1;
        used[2] = 1;
        used[3] = 1;
    }

    /* if already computed, return */
    if (n <= seq_len)
        return seq[n - 1];

    /* compute missing terms up to n */
    for (idx_t idx = seq_len + 1; idx <= n; ++idx) {
        /* sum of the three preceding terms */
        seq_t sum = seq[idx - 2] + seq[idx - 3] + seq[idx - 4];

        /* find smallest unused candidate w/ gcd>1 with sum */
        seq_t cand_val = 0;
        for (idx_t cand = 1; ; ++cand) {
            ensure_used_cap(cand);
            if (used[(size_t)cand]) continue;          /* already used   */
            if (gcd((seq_t)cand, sum) > (seq_t)1) {     /* shares a factor */
                cand_val = (seq_t)cand;
                used[(size_t)cand] = 1;                /* mark now used  */
                break;
            }
        }

        /* store the new term and grow the array if needed */
        ensure_seq_cap(idx);
        seq[idx - 1] = cand_val;
        seq_len = idx;
    }

    return seq[n - 1];
}
