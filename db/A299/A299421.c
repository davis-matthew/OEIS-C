/*
 * A299421  Solution b( ) of the complementary equation a(n) = b(n-1) +
 *          b(n-2), where a(0) = 4, a(1) = 5; see Comments.
 * https://oeis.org/A299421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 169, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   ensure_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A299421 – complementary sequence b(n)
    Defined by:
        a(0) = 4,  a(1) = 5
        a(n) = b(n‑1) + b(n‑2)   for n ≥ 2
        b(0) = least positive integer not in {a(0),a(1)}
        b(n) = least positive integer not in {a(0)…a(n), b(0)…b(n‑1)}   for n > 1
    The returned value seq_t a(idx_t n) is b(n).  The sequence starts
    with 1, 2, 6, 7, 9, … as listed in the OEIS entry.  The algorithm
    pre‑computes both sequences, keeping a dynamic set of used integers
    in a bit‑array for fast look‑up.  The implementation works for any
    seq_t and idx_t that are unsigned integer types.  All allocations
    use explicit casts so that the code is valid in both C11 and C++17.  */
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/ensure_seq.h"

/* Ensure the used bit‑array can hold a value ‘val’.  */
static void ensure_used(idx_t val, unsigned char **used, idx_t *used_size)
{
    if (val >= *used_size) {
        idx_t new_size = val + 1;
        unsigned char *tmp = (unsigned char*)realloc(*used, (size_t)new_size);
        if (!tmp) exit(1);                       /* out of memory */
        /* initialise new slots to 0 */
        memset(tmp + (size_t)*used_size, 0,
               (size_t)(new_size - *used_size));
        *used = tmp;
        *used_size = new_size;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static state shared among all calls */
    static seq_t *a_vals = NULL;   /* holds a(0), a(1), … */
    static seq_t *b_vals = NULL;   /* holds b(0), b(1), … – this is the returned sequence */
    static size_t a_cap = 0, b_cap = 0, a_len = 0, b_len = 0;
    static unsigned char *used = NULL;
    static idx_t used_size = 0;
    static idx_t cand = 1;          /* candidate for the next unused integer */

    /* one‑time initialization */
    if (!a_vals) {
        a_cap = 4;
        b_cap = 4;
        a_len = 2;      /* a(0) .. a(1) */
        b_len = 2;      /* b(0) .. b(1) */
        a_vals = (seq_t*)calloc(a_cap, sizeof(seq_t));
        b_vals = (seq_t*)calloc(b_cap, sizeof(seq_t));
        if (!a_vals || !b_vals) exit(1);   /* memory failure */
        /* a(0)=4, a(1)=5 */
        a_vals[0] = (seq_t)4;
        a_vals[1] = (seq_t)5;
        /* used bit‑array: start with enough space for numbers up to 5 */
        used = (unsigned char*)calloc(6, 1);  /* indices 0..5 */
        used_size = 6;
        if (!used) exit(1);
        used[4] = 1;     /* a(0) */
        used[5] = 1;     /* a(1) */
        /* b(0)=1 */
        b_vals[0] = (seq_t)1;
        ensure_used(1, &used, &used_size);
        used[1] = 1;
        cand = 2;
        /* b(1)=2 */
        b_vals[1] = (seq_t)2;
        ensure_used(2, &used, &used_size);
        used[2] = 1;
        cand = 3;
    }

    /* extend the sequences up to index n (if needed) */
    if (b_len <= n) {
        for (size_t i = b_len; i <= (size_t)n; ++i) {
            /* compute a(i) = b(i-1) + b(i-2)    */
            seq_t ai = b_vals[i-1] + b_vals[i-2];

            /* store a(i) */
            ensure_seq(&a_vals, &a_cap, a_len + 1);
            a_vals[a_len++] = ai;

            /* mark ai as used */
            idx_t ai_idx = (idx_t)ai;
            ensure_used(ai_idx, &used, &used_size);
            used[(size_t)ai_idx] = 1;

            /* find next unused candidate for b(i) */
            while (cand < used_size && used[(size_t)cand]) ++cand;
            if (cand >= used_size) {
                ensure_used(cand, &used, &used_size);
            }

            /* store b(i) */
            ensure_seq(&b_vals, &b_cap, i + 1);
            b_vals[i] = (seq_t)cand;
            used[(size_t)cand] = 1;
            ++cand;              /* update candidate for next step */

            b_len = i + 1;
        }
    }

    return b_vals[n];
}
