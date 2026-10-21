/*
 * A269932  Number of n X 2 0..6 arrays with some element plus some
 *          horizontally, diagonally, antidiagonally or vertically adjacent
 *          neighbor totalling six exactly once.
 * https://oeis.org/A269932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A269932: Number of n×2 arrays with entries 0..6 that have exactly one pair
   (horizontal, vertical, diagonal or anti‑diagonal) of adjacent cells summing
   to 6.  The function computes this value by dynamic programming over the
   49 possible rows.  */

/* The implementation is fully compatible with seq_t being any unsigned
   integer type or an arbitrary‑precision integer class, and with idx_t==uint64_t. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pre‑compute the 49 possible row vectors (a,b) and whether they contain
       a horizontal pair summing to 6.  These tables are static for efficiency. */
    static bool initialized = false;
    static int rowA[49];
    static int rowB[49];
    static int horiz6[49];   /* 1 if a+b==6, else 0 */

    if (!initialized) {
        for (int s = 0; s < 49; ++s) {
            int a_val = s / 7;
            int b_val = s % 7;
            rowA[s] = a_val;
            rowB[s] = b_val;
            horiz6[s] = (a_val + b_val == 6) ? 1 : 0;
        }
        initialized = true;
    }

    /* dp0[s] – number of ways to reach state s with 0 six‑sum edges seen
       dp1[s] – number of ways to reach state s with exactly 1 six‑sum edge seen */
    seq_t dp0[49] = {0};
    seq_t dp1[49] = {0};

    /* initialise for the first row */
    for (int s = 0; s < 49; ++s) {
        if (horiz6[s])
            dp1[s] = 1;
        else
            dp0[s] = 1;
    }

    if (n == 1) {
        seq_t res = 0;
        for (int s = 0; s < 49; ++s) res += dp1[s];
        return res;
    }

    /* temporary arrays for the next row */
    seq_t next0[49];
    seq_t next1[49];

    /* iterate over the remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear next arrays */
        for (int s = 0; s < 49; ++s) next0[s] = next1[s] = 0;

        for (int prev = 0; prev < 49; ++prev) {
            seq_t cnt0 = dp0[prev];
            seq_t cnt1 = dp1[prev];
            if (!cnt0 && !cnt1) continue;

            int a = rowA[prev];
            int b = rowB[prev];

            for (int cur = 0; cur < 49; ++cur) {
                int c = rowA[cur];
                int d = rowB[cur];

                /* count the number of new six‑sum edges created between
                   the previous and current row and within the current row */
                int add = 0;
                if (a + c == 6) ++add;   /* vertical */
                if (b + d == 6) ++add;   /* vertical */
                if (a + d == 6) ++add;   /* anti‑diagonal */
                if (b + c == 6) ++add;   /* diagonal */
                add += horiz6[cur];      /* horizontal in the current row */

                /* propagate DP states while respecting the exactly‑one constraint */
                if (cnt0) {
                    if (add == 0) next0[cur] += cnt0;
                    else if (add == 1) next1[cur] += cnt0;
                }
                if (cnt1) {
                    if (add == 0) next1[cur] += cnt1;
                    /* add > 0 would produce >1 six‑sum edges – skip */
                }
            }
        }

        /* copy next to dp for the next iteration */
        for (int s = 0; s < 49; ++s) {
            dp0[s] = next0[s];
            dp1[s] = next1[s];
        }
    }

    /* sum over states with exactly one six‑sum edge */
    seq_t result = 0;
    for (int s = 0; s < 49; ++s) result += dp1[s];
    return result;
}
