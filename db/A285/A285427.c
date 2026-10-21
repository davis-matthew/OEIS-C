/*
 * A285427  Fixed point of the morphism 0->11, 1-> 100.
 * https://oeis.org/A285427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth term of the fixed point of the morphism 0 → 11,
 *            1 → 100. The sequence is the infinite binary word obtained by
 *            iterating this morphism starting from 0 and reading the limit
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Computes one term of the sequence. */
seq_t a(idx_t n)
{
    /* Ensure n is at least 1 (offset is 1).  For n <= 0 we return 0. */
    if (n == 0) return (seq_t)0;

    /* Convert to seq_t for calculations. */
    const seq_t n_seq = (seq_t)n;

    /* Preallocate a reasonably large table of lengths. */
    /* 200 levels is far more than enough for n up to 10^12 and beyond. */
    enum { MAX_H = 200 };
    static seq_t len0[MAX_H];
    static seq_t len1[MAX_H];

    /* Base lengths (after 0 iterations). */
    len0[0] = (seq_t)1;
    len1[0] = (seq_t)1;

    /* Find the smallest height H such that len0[H] ≥ n_seq.
       We also enforce H >= 1 as the first symbol of the limit appears
       after the first substitution. */
    size_t H = 0;
    while (len0[H] < n_seq) {
        ++H;
        len0[H] = (seq_t)2 * len1[H - 1];
        len1[H] = len1[H - 1] + (seq_t)2 * len0[H - 1];
    }
    if (H == 0) H = 1;

    int prev_val = -1;            /* previous computed value */
    int cur_val;                  /* current computed value */

    for (;;) {
        /* Compute the symbol at position n_seq using the current depth H. */
        int sym = 0;              /* start from the root symbol 0 */
        size_t level = H;
        seq_t idx = n_seq;

        while (level > 0) {
            if (sym == 0) {
                /* The expansion of 0 is [1, 1].  Both children are 1. */
                seq_t child_len = len1[level - 1];
                if (idx <= child_len) {
                    sym = 1;
                } else {
                    idx -= child_len;
                    sym = 1;
                }
            } else { /* sym == 1 */
                /* The expansion of 1 is [1, 0, 0]. */
                seq_t len_child1 = len1[level - 1];
                seq_t len_child0 = len0[level - 1];

                if (idx <= len_child1) {
                    sym = 1;                     /* first child is 1 */
                } else if (idx <= len_child1 + len_child0) {
                    idx -= len_child1;             /* second child is 0 */
                    sym = 0;
                } else {
                    idx -= len_child1 + len_child0; /* third child is 0 */
                    sym = 0;
                }
            }
            --level;
        }

        cur_val = sym;          /* 0 or 1 */
        if (cur_val == prev_val) {
            return (seq_t)cur_val;   /* value has stabilized */
        }

        /* Value not yet stable: increase the depth and recompute. */
        prev_val = cur_val;
        if (H + 1 >= MAX_H) {
            /* This should never happen for the intended test range. */
            break;
        }
        ++H;
        len0[H] = (seq_t)2 * len1[H - 1];
        len1[H] = len1[H - 1] + (seq_t)2 * len0[H - 1];
    }

    /* Fallback: return the current value (should not reach here). */
    return (seq_t)cur_val;
}
