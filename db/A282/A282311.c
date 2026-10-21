/*
 * A282311  Number of n X 3 0..1 arrays with no 1 equal to more than four of
 *          its king-move neighbors.
 * https://oeis.org/A282311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computation of A282311 via dynamic programming over rows. */
seq_t a(idx_t n)
{
    /* Handle out-of-range (sentinel) but OEIS specifies offset 1. */
    if (n == 0) return (seq_t)0;

    /* Constants for the DP. */
    enum { PATS = 8, KEYS = 216 };  /* 2^3 patterns, 0..5^3 states */

    /* Precompute neighbor contributions from a row above and the same row. */
    static unsigned char partial[PATS][PATS][3];
    static bool partial_inited = false;
    if (!partial_inited) {
        for (int prev = 0; prev < PATS; ++prev) {
            for (int cur  = 0; cur  < PATS; ++cur) {
                for (int j = 0; j < 3; ++j) {
                    unsigned char cnt = 0;
                    /* Contributions from the row above. */
                    if (prev & (1 << j))                ++cnt;
                    if (j > 0 && (prev & (1 << (j-1)))) ++cnt;
                    if (j < 2 && (prev & (1 << (j+1)))) ++cnt;
                    /* Contributions from the same row (left/right). */
                    if (j > 0 && (cur  & (1 << (j-1)))) ++cnt;
                    if (j < 2 && (cur  & (1 << (j+1)))) ++cnt;
                    partial[prev][cur][j] = cnt;
                }
            }
        }
        partial_inited = true;
    }

    /* DP state arrays.  Static to avoid reallocation per call. */
    static seq_t dp_cur[PATS][KEYS];
    static seq_t dp_next[PATS][KEYS];
    /* Zero the current DP for the first row. */
    for (int p = 0; p < PATS; ++p)
        for (int k = 0; k < KEYS; ++k)
            dp_cur[p][k] = (seq_t)0;

    /* Initialize first row: previous pattern is all zeros. */
    for (int cur = 0; cur < PATS; ++cur) {
        unsigned char c0 = partial[0][cur][0];
        unsigned char c1 = partial[0][cur][1];
        unsigned char c2 = partial[0][cur][2];
        int key = c0 + 6 * (c1 + 6 * c2);
        dp_cur[cur][key] += (seq_t)1;
    }

    /* Process rows 2 .. n. */
    for (idx_t r = 2; r <= n; ++r) {
        /* Clear dp_next. */
        for (int p = 0; p < PATS; ++p)
            for (int k = 0; k < KEYS; ++k)
                dp_next[p][k] = (seq_t)0;

        /* Transition from each state (prev, key). */
        for (int prev = 0; prev < PATS; ++prev) {
            for (int key = 0; key < KEYS; ++key) {
                seq_t val = dp_cur[prev][key];
                if (val == (seq_t)0) continue;

                /* Decode partial counts for previous row. */
                unsigned char c[3];
                c[0] = key % 6;
                c[1] = (key / 6) % 6;
                c[2] = key / 36;

                for (int next = 0; next < PATS; ++next) {
                    /* Check that every 1 in the previous row
                       does not exceed 4 neighbors after adding below row. */
                    bool ok = true;
                    for (int j = 0; j < 3; ++j) {
                        unsigned char below = 0;
                        if ((next >> j) & 1)                 ++below;
                        if (j > 0 && ((next >> (j-1)) & 1))  ++below;
                        if (j < 2 && ((next >> (j+1)) & 1))  ++below;
                        int total = c[j] + below;
                        if ((prev >> j) & 1 && total > 4) { ok = false; break; }
                    }
                    if (!ok) continue;

                    /* New partial counts for the 'next' row. */
                    unsigned char nc0 = partial[prev][next][0];
                    unsigned char nc1 = partial[prev][next][1];
                    unsigned char nc2 = partial[prev][next][2];
                    int nkey = nc0 + 6 * (nc1 + 6 * nc2);

                    dp_next[next][nkey] += val;
                }
            }
        }

        /* Move dp_next to dp_cur for the next iteration. */
        for (int p = 0; p < PATS; ++p)
            for (int k = 0; k < KEYS; ++k)
                dp_cur[p][k] = dp_next[p][k];
    }

    /* Sum over final states: ensure bottom row also satisfies the limit. */
    seq_t ans = (seq_t)0;
    for (int p = 0; p < PATS; ++p) {
        for (int key = 0; key < KEYS; ++key) {
            seq_t val = dp_cur[p][key];
            if (val == (seq_t)0) continue;
            unsigned char c0 = key % 6;
            unsigned char c1 = (key / 6) % 6;
            unsigned char c2 = key / 36;
            bool ok = true;
            if ((p >> 0) & 1 && c0 > 4) ok = false;
            if ((p >> 1) & 1 && c1 > 4) ok = false;
            if ((p >> 2) & 1 && c2 > 4) ok = false;
            if (ok) ans += val;
        }
    }
    return ans;
}
