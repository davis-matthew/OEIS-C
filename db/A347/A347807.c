/*
 * A347807  Expansion of (theta_3(x) - 1)^4 / (8 * (3 - theta_3(x))).
 * https://oeis.org/A347807
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of compositions of n into 4 or more squares.
   Computed by dynamic programming with memoisation. */

#include <stdlib.h>
#include <stdbool.h>

/* The types seq_t (value) and idx_t (index) are defined by the build system. */

seq_t a(idx_t n)
{
    /* ---- Persistent tables for memoisation ---- */
    static seq_t *dp_total = NULL;   /* all compositions into squares              */
    static seq_t *dp2      = NULL;   /* compositions into exactly 2 squares       */
    static seq_t *dp3      = NULL;   /* compositions into exactly 3 squares       */
    static bool   *is_sq   = NULL;   /* true iff the index is a perfect square    */
    static idx_t   max_n   = 0;       /* highest n we have computed so far         */

    /* ---- Resize tables if a larger n is requested ---- */
    if (n > max_n) {
        size_t new_size = (size_t)(n + 1);                /* indices 0 .. n */

        dp_total = (seq_t*)realloc(dp_total, new_size * sizeof(seq_t));
        dp2      = (seq_t*)realloc(dp2,      new_size * sizeof(seq_t));
        dp3      = (seq_t*)realloc(dp3,      new_size * sizeof(seq_t));
        is_sq    = (bool*)realloc(is_sq,    new_size * sizeof(bool));

        if (!dp_total || !dp2 || !dp3 || !is_sq) abort(); /* allocation failure */

        /* Initial state for index 0 */
        if (max_n == 0) {
            dp_total[0] = (seq_t)1;   /* empty composition */
            dp2[0]      = 0;
            dp3[0]      = 0;
            is_sq[0]    = false;      /* 0 is not a positive square */
        }

        /* Compute values up to the new maximum */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t sum_total = 0;
            seq_t sum_2     = 0;
            seq_t sum_3     = 0;

            /* Iterate over all square integers s <= i */
            for (idx_t j = 1; j * j <= i; ++j) {
                idx_t sq = j * j;

                /* All compositions: first part = sq, rest = composition of i-sq */
                sum_total += dp_total[i - sq];

                /* Two‑square compositions: remainder must be a square */
                if (is_sq[i - sq]) {
                    ++sum_2;
                }

                /* Three‑square compositions: first part = sq, rest into 2 squares */
                sum_3 += dp2[i - sq];
            }

            dp_total[i] = sum_total;
            dp2[i]      = sum_2;
            dp3[i]      = sum_3;

            /* Determine whether i itself is a perfect square */
            idx_t r = 1;
            while (r * r < i) ++r;
            is_sq[i] = (r * r == i);
        }

        max_n = n;
    }

    /* ---- Compute the required value ---- */
    if (n < 4) /* undefined for n < 4 according to the OEIS offset */
        return (seq_t)0;

    /* Subtract compositions with 1, 2, or 3 squares */
    seq_t result = dp_total[n];
    if (is_sq[n]) result -= (seq_t)1;  /* 1‑square composition */
    result -= dp2[n];
    result -= dp3[n];
    return result;
}
