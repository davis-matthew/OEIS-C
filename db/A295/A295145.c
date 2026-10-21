/*
 * A295145  Solution of the complementary equation a(n) = a(n-1) + 2*a(n-2) +
 *          b(n-2), where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4, and (a(n))
 *          and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + b(n-2) a(0) = 1, a(1) = 2 b(0) = 3
 *            (the first positive integer not equal to a(0), a(1))
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* sequence computation ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static storage for memoised values */
    static seq_t *a_vals = NULL;
    static size_t a_cap  = 0;
    static idx_t  a_len  = 0;

    static seq_t *b_vals  = NULL;
    static size_t b_cap  = 0;
    static idx_t  b_len  = 0;

    /* the next integer to test for being a missing (b) value */
    static seq_t next_candidate = 0;

    /* ----- initialization ------------------------------------------------- */
    if (a_vals == NULL) {
        /* base a(0), a(1) */
        a_cap = 2;
        a_vals = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_vals[0] = (seq_t)1;
        a_vals[1] = (seq_t)2;
        a_len   = 2;

        /* initialise first b value */
        b_cap = 1;
        b_vals = (seq_t *)malloc(b_cap * sizeof(seq_t));
        seq_t cand = (seq_t)1;
        /* skip a(0) and a(1) */
        while (cand == a_vals[0] || cand == a_vals[1]) cand++;
        b_vals[0] = cand;
        b_len = 1;
        next_candidate = cand + (seq_t)1;
    }

    /* desired number of b values for computing up to 'n' */
    idx_t needed_b = (n >= 1) ? (n - 1) : 0;

    /* ----- compute terms up to 'n' -------------------------------------- */
    while (a_len <= n) {
        /* generate b values needed for the current recurrence term */
        while (b_len <= a_len - (idx_t)2) {
            /* find the next integer not present in a_vals[0 .. a_len-1] */
            seq_t cand = next_candidate;
            bool found;
            do {
                found = false;
                for (idx_t i = 0; i < a_len; ++i) {
                    if (a_vals[i] == cand) {
                        found = true;
                        break;
                    }
                }
                if (found) cand++;
            } while (found);

            /* store new b value */
            if (b_len >= b_cap) {
                size_t new_cap = (b_cap == 0) ? 4 : b_cap << 1;
                b_vals = (seq_t *)realloc(b_vals, new_cap * sizeof(seq_t));
                b_cap = new_cap;
            }
            b_vals[b_len++] = cand;
            next_candidate = cand + (seq_t)1;
        }

        /* ensure capacity for new a value */
        if (a_len >= a_cap) {
            size_t new_cap = (a_cap == 0) ? 4 : a_cap << 1;
            a_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
            a_cap = new_cap;
        }

        /* compute a[a_len] */
        seq_t val = a_vals[a_len - 1]            /* a(n-1) */
                  + ((seq_t)2 * a_vals[a_len - 2]) /* 2*a(n-2) */
                  + b_vals[a_len - 2];              /* b(n-2) */
        a_vals[a_len++] = val;
    }

    return a_vals[n];
}
