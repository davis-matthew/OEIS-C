/*
 * A284079  Number of 5 X n 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly two elements.
 * https://oeis.org/A284079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n binary arrays with exactly two 1‑cells that
 *            * have more than three of their horizontal, diagonal and
 *            antidiagonal * neighbors equal to 1, all other 1‑cells having at
 *            most three such neighbors
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Transition table entry */
static struct TransList {
    int next_idx[32];
    int add_val[32];
    int cnt;
} Trans[1024];

/* Table of violations contributed by a column */
static uint8_t AddTable[32][32][32];

/* Count violations in column 'cur' given the left column 'prev' and
 * the right column 'next'. */
static inline uint8_t
count_violations(uint8_t prev, uint8_t cur, uint8_t next)
{
    uint8_t v = 0;
    for (int r = 0; r < 5; ++r) {
        if ((cur >> r) & 1U) {
            uint8_t neigh = 0;
            /* left */
            if ((prev >> r) & 1U) ++neigh;
            /* right */
            if ((next >> r) & 1U) ++neigh;
            /* up-left */
            if (r > 0 && ((prev >> (r - 1)) & 1U)) ++neigh;
            /* down-left */
            if (r < 4 && ((prev >> (r + 1)) & 1U)) ++neigh;
            /* up-right */
            if (r > 0 && ((next >> (r - 1)) & 1U)) ++neigh;
            /* down-right */
            if (r < 4 && ((next >> (r + 1)) & 1U)) ++neigh;
            if (neigh > 3U) ++v;
        }
    }
    return v;
}

/* Pre‑compute AddTable and transition lists.  The routine is executed only
 * once because 'initialized' is static. */
static void
build_tables(void)
{
    static bool initialized = false;
    if (initialized)
        return;
    for (int p = 0; p < 32; ++p) {
        for (int c = 0; c < 32; ++c) {
            for (int n = 0; n < 32; ++n) {
                AddTable[p][c][n] = count_violations((uint8_t)p, (uint8_t)c,
                                                     (uint8_t)n);
            }
        }
    }
    /* Build transition lists.  At most two violations per column are
     * allowed, otherwise the total would exceed the required two. */
    for (int p2 = 0; p2 < 32; ++p2) {
        for (int p1 = 0; p1 < 32; ++p1) {
            int idx = (p2 << 5) | p1;
            int c = 0;
            for (int n = 0; n < 32; ++n) {
                uint8_t add = AddTable[p2][p1][n];
                if (add <= 2U) {
                    Trans[idx].next_idx[c] = (p1 << 5) | n;
                    Trans[idx].add_val[c] = add;
                    ++c;
                }
            }
            Trans[idx].cnt = c;
        }
    }
    initialized = true;
}

/* Main DP routine.  DP over the last two columns; each state keeps the
 * number of violations counted so far (0, 1 or 2). */
seq_t a(idx_t n)
{
    if (n < 3U)
        return (seq_t)0; /* a(1)=a(2)=0 */

    build_tables();

    static seq_t dp[1024][3];
    static seq_t ndp[1024][3];

    /* Initialise dp with columns 0 and 1. */
    for (int i = 0; i < 1024; ++i)
        for (int v = 0; v < 3; ++v)
            dp[i][v] = 0;

    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            uint8_t v0 = AddTable[0][a][b];        /* violations in column 0 */
            if (v0 <= 2U) {
                int idx = (a << 5) | b;              /* state: (col0, col1) */
                dp[idx][v0] += (seq_t)1;
            }
        }
    }

    /* Process columns 2 … n-1. */
    for (idx_t col = 2U; col < n; ++col) {
        for (int i = 0; i < 1024; ++i)
            for (int v = 0; v < 3; ++v)
                ndp[i][v] = 0;

        for (int idx = 0; idx < 1024; ++idx) {
            for (int v = 0; v < 3; ++v) {
                seq_t cnt = dp[idx][v];
                if (cnt == 0)
                    continue;
                const struct TransList *t = &Trans[idx];
                for (int k = 0; k < t->cnt; ++k) {
                    uint8_t add = (uint8_t)t->add_val[k];
                    uint8_t nv = (uint8_t)(v + add);
                    if (nv > 2U)
                        continue;
                    int nidx = t->next_idx[k];
                    ndp[nidx][nv] += cnt;
                }
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i < 1024; ++i)
            for (int v = 0; v < 3; ++v)
                dp[i][v] = ndp[i][v];
    }

    /* Add violations of the last column (right side absent). */
    seq_t ans = 0;
    for (int idx = 0; idx < 1024; ++idx) {
        int p2 = idx >> 5;
        int p1 = idx & 31;
        uint8_t last_viol = AddTable[p2][p1][0];
        for (int v = 0; v < 3; ++v) {
            seq_t cnt = dp[idx][v];
            if (cnt == 0)
                continue;
            if ((uint8_t)(v + last_viol) == 2U)
                ans += cnt;
        }
    }
    return ans;
}
