/*
 * A296825  Number of nX6 0..1 arrays with each 1 adjacent to 2, 3 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence implementation: dynamic programming over rows
 * with king‑move adjacency constraints.
 * a(n) = number of n×6 binary matrices where each 1 has
 * 2, 3 or 4 neighboring 1’s counting only existing cells.
 * Off‑diagonal entries of the transition table are pre‑computed
 * for speed.  The algorithm has O(n·64²) time and O(1) memory
 * apart from a small fixed table.
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* Transition mask: for each (prev, cur) the set of possible next
 * rows that make 'cur' valid given prev and next.  A bit i is set
 * iff row i can follow.  Stored as a 64‑bit mask. */
static unsigned long long next_mask[64 * 64];
static bool tables_built = false;

/* Test whether a middle row 'mid' is valid given its neighbours
 * 'above' and 'below'.  All 1 bits in 'mid' must have between
 * 2 and 4 neighbouring 1 bits among the king‑move positions that
 * exist. */
static bool row_valid(int above, int mid, int below)
{
    for (int col = 0; col < 6; ++col) {
        if (!(mid & (1 << col)))
            continue;                 /* no 1 here */

        int nb = 0;
        /* same row */
        if (col > 0 && (mid & (1 << (col - 1)))) ++nb;
        if (col < 5 && (mid & (1 << (col + 1)))) ++nb;
        /* row above */
        if (col > 0 && (above & (1 << (col - 1)))) ++nb;
        if (above & (1 << col))                     ++nb;
        if (col < 5 && (above & (1 << (col + 1)))) ++nb;
        /* row below */
        if (col > 0 && (below & (1 << (col - 1)))) ++nb;
        if (below & (1 << col))                     ++nb;
        if (col < 5 && (below & (1 << (col + 1)))) ++nb;

        if (nb < 2 || nb > 4)
            return false;
    }
    return true;
}

/* Build the transition table once.  After this, next_mask[p*64+c]
 * contains the mask of rows that can follow when previous row
 * has mask p and current row has mask c. */
static void build_tables(void)
{
    if (tables_built)
        return;
    for (int prev = 0; prev < 64; ++prev) {
        for (int cur = 0; cur < 64; ++cur) {
            unsigned long long mask = 0;
            for (int next = 0; next < 64; ++next) {
                if (row_valid(prev, cur, next))
                    mask |= 1ULL << next;
            }
            next_mask[prev * 64 + cur] = mask;
        }
    }
    tables_built = true;
}

/* Main sequence function.  idx_t and seq_t are supplied by the
 * build system and must not be defined here. */
seq_t a(idx_t n)
{
    if (n == 0)          /* not used by OEIS but safe for completeness */
        return (seq_t)0;

    build_tables();

    static seq_t dp[64][64];   /* dp[prev][cur] – counts so far */
    static seq_t next[64][64];
    memset(dp, 0, sizeof(dp));

    /* Initial state: dummy previous row of all zeros, first row (cur)
       is not yet validated.  Every mask is allowed. */
    for (int cur = 0; cur < 64; ++cur)
        dp[0][cur] = (seq_t)1;

    if (n == 1) {
        seq_t res = 0;
        for (int cur = 0; cur < 64; ++cur) {
            if (next_mask[0 * 64 + cur] & 1ULL)  /* next = 0 satisfies validation */
                res += dp[0][cur];
        }
        return res;
    }

    /* Process rows 2 … n */
    for (idx_t row = 2; row <= n; ++row) {
        memset(next, 0, sizeof(next));
        for (int prev = 0; prev < 64; ++prev) {
            for (int cur = 0; cur < 64; ++cur) {
                seq_t ways = dp[prev][cur];
                if (!ways)
                    continue;
                unsigned long long m = next_mask[prev * 64 + cur];
                while (m) {                 /* iterate set bits efficiently */
                    int nxt = __builtin_ctzll(m);
                    next[cur][nxt] += ways;
                    m &= m - 1;
                }
            }
        }
        memcpy(dp, next, sizeof(dp));
    }

    /* Final validation: the last row (cur) must be valid with below = 0. */
    seq_t result = 0;
    for (int prev = 0; prev < 64; ++prev) {
        for (int cur = 0; cur < 64; ++cur) {
            seq_t ways = dp[prev][cur];
            if (!ways)
                continue;
            if (next_mask[prev * 64 + cur] & 1ULL)  /* bit 0 set => below=0 valid */
                result += ways;
        }
    }
    return result;
}
