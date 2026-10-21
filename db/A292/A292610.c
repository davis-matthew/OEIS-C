/*
 * A292610  Take 3 triangle numbers, skip 1 triangle number, take 4 triangle
 *          numbers, skip 2 triangle numbers, take 5 triangle numbers, skip 3
 *          triangle numbers, ...
 * https://oeis.org/A292610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 207, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A292610: take 3 triangle numbers, skip 1, take 4, skip 2,
   take 5, skip 3, and so on.  The nth term is the nth selected
   triangular number. */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* n is ≥1 by specification */
    idx_t need = n;          /* how many terms still to pick */
    idx_t m = 1;             /* current triangular index (T(m)) */
    idx_t take_len = 3;      /* current block length to take */
    idx_t take_left = take_len; /* remaining to take in current block */
    idx_t skip_left = 0;     /* remaining to skip in current block */
    bool taking = true;      /* true if we are currently taking */

    while (true) {
        /* current triangular number T(m) = m*(m+1)/2 */
        seq_t term = ((seq_t)m * ((seq_t)m + 1)) / (seq_t)2;

        if (taking) {
            /* the term is taken: count it */
            if (--need == 0)
                return term;

            /* move to the next triangular number and
               update remaining count in this block         */
            if (--take_left == 0) {
                /* block ended: switch to skipping */
                taking = false;
                skip_left = take_len - 2;  /* skip length = take_len - 2 */
            }
        } else {
            /* the term is skipped; just update skip counter */
            if (--skip_left == 0) {
                /* skip block ended: prepare next take block */
                taking = true;
                take_len++;          /* block length grows by 1 each cycle */
                take_left = take_len;
            }
        }

        /* advance to next triangular number */
        m++;
    }
}
