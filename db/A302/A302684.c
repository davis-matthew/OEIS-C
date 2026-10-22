/*
 * A302684  Number of 6Xn 0..1 arrays with every element equal to 0, 1, 4 or 6
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence A302684 - computed by dynamic programming followed by a
 *            linear recurrence of order 17 for n>23
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Compatibility with both C11 and C++17:
   The types seq_t and idx_t are defined externally by the build system. */

static const unsigned int NONE = 64u; /* sentinel for "no column" */

/* Precomputed masks and tables for transitions */
static uint64_t leftMissingMask[64];          /* allowed next columns when left neighbor missing */
static uint64_t compatMask[64][64];           /* allowed next columns when both neighbors present   */
static bool rightMissing[64][64];             /* whether a column satisfies its condition when
                                               the right neighbor is missing */

/* Check if a column satisfies the adjacency condition given its left
   and right neighboring columns. prev/right==NONE indicates missing. */
static bool
check_column(unsigned int prev, unsigned int cur, unsigned int next)
{
    int r;
    for (r = 0; r < 6; ++r) {
        int val = (cur >> r) & 1;
        int cnt = 0;
        if (prev != NONE) {
            if (((prev >> r) & 1) == val) ++cnt;
            if (r > 0 && (((prev >> (r - 1)) & 1) == val)) ++cnt;
            if (r < 5 && (((prev >> (r + 1)) & 1) == val)) ++cnt;
        }
        if (next != NONE) {
            if (((next >> r) & 1) == val) ++cnt;
            if (r > 0 && (((next >> (r - 1)) & 1) == val)) ++cnt;
            if (r < 5 && (((next >> (r + 1)) & 1) == val)) ++cnt;
        }
        if (cnt != 0 && cnt != 1 && cnt != 4 && cnt != 6) return false;
    }
    return true;
}

/* Precompute masks and tables only once. */
static void
precompute_masks(void)
{
    static bool ready = false;
    if (ready) return;
    ready = true;

    unsigned int prev, cur, next;
    for (prev = 0; prev < 64; ++prev) {
        for (cur = 0; cur < 64; ++cur) {
            rightMissing[prev][cur] = check_column(prev, cur, NONE);
            compatMask[prev][cur] = 0;
            for (next = 0; next < 64; ++next)
                if (check_column(prev, cur, next))
                    compatMask[prev][cur] |= (uint64_t)1 << next;
        }
    }
    for (cur = 0; cur < 64; ++cur) {
        leftMissingMask[cur] = 0;
        for (next = 0; next < 64; ++next)
            if (check_column(NONE, cur, next))
                leftMissingMask[cur] |= (uint64_t)1 << next;
    }
}

/* Compute the first 24 terms by dynamic programming using the
   transfer‑matrix method described above. */
static void
initial_terms(seq_t *out)
{
    precompute_masks();

    /* dp[prev][cur] holds the number of ways to build columns up to cur
       with left neighbor prev.  Index NONE (64) represents a missing column. */
    static seq_t dp[65][65];
    static seq_t newdp[65][65];
    unsigned int prev, cur, next;
    /* initialise dp with the first column (col 0). */
    for (prev = 0; prev < 65; ++prev)
        for (cur = 0; cur < 65; ++cur)
            dp[prev][cur] = 0;
    for (cur = 0; cur < 64; ++cur) {
        if ((cur & 1) == 0) /* upper left element must be 0 */
            dp[NONE][cur] = (seq_t)1;
    }
    /* compute a(1) : evaluate column 0 with no neighbours on either side. */
    seq_t val1 = 0;
    for (cur = 0; cur < 64; ++cur)
        if (check_column(NONE, cur, NONE))
            val1 += dp[NONE][cur];
    out[1] = val1;

    /* iterate for columns 1..23 to compute a(2)..a(24) */
    for (unsigned int step = 1; step < 24; ++step) {
        /* clear newdp */
        for (prev = 0; prev < 65; ++prev)
            for (cur = 0; cur < 65; ++cur)
                newdp[prev][cur] = 0;
        /* transition */
        for (prev = 0; prev <= 64; ++prev) {
            for (cur = 0; cur < 64; ++cur) {
                seq_t cnt = dp[prev][cur];
                if (!cnt) continue;
                uint64_t mask = (prev == NONE) ? leftMissingMask[cur] : compatMask[prev][cur];
                while (mask) {
                    int nidx = __builtin_ctzll(mask);
                    mask &= mask - 1;
                    newdp[cur][nidx] += cnt;
                }
            }
        }
        /* compute a(step+1) by evaluating the last column with right neighbor missing */
        seq_t val = 0;
        for (prev = 0; prev < 64; ++prev)
            for (cur = 0; cur < 64; ++cur)
                if (rightMissing[prev][cur])
                    val += newdp[prev][cur];
        out[step + 1] = val;
        /* copy newdp into dp for next iteration */
        for (prev = 0; prev <= 64; ++prev)
            for (cur = 0; cur < 65; ++cur)
                dp[prev][cur] = newdp[prev][cur];
    }
}

/* Main function that returns the n-th term of the sequence. */
seq_t
a(idx_t n)
{
    static seq_t base[25];   /* a(1) .. a(24) */
    static bool first = true;
    if (first) {
        first = false;
        initial_terms(base);
    }

    /* For n <= 24, return the precomputed value. */
    if (n <= 24) return base[(idx_t)n];

    /* For larger n, use the empirical recurrence of order 17. */
    /* We memoise the computed values so repeated calls are efficient. */
    static seq_t *rec = NULL;
    static idx_t max_n = 24;

    if (!rec) {
        rec = (seq_t *)malloc((max_n + 1) * sizeof(seq_t));
        for (idx_t i = 1; i <= max_n; ++i) rec[i] = base[(idx_t)i];
    }
    if (n > max_n) {
        rec = (seq_t *)realloc(rec, (n + 1) * sizeof(seq_t));
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t v = 0;
            v += rec[i - 1];
            v += (seq_t)2 * rec[i - 2];
            v += (seq_t)18 * rec[i - 3];
            v += (seq_t)31 * rec[i - 4];
            v -= (seq_t)4 * rec[i - 5];
            v -= (seq_t)56 * rec[i - 6];
            v -= (seq_t)58 * rec[i - 7];
            v -= (seq_t)38 * rec[i - 8];
            v += (seq_t)52 * rec[i - 9];
            v += (seq_t)7 * rec[i - 10];
            v -= (seq_t)47 * rec[i - 11];
            v += (seq_t)10 * rec[i - 12];
            v += rec[i - 13];
            v -= (seq_t)32 * rec[i - 14];
            v += (seq_t)12 * rec[i - 15];
            v += (seq_t)2 * rec[i - 16];
            v += (seq_t)8 * rec[i - 17];
            rec[i] = v;
        }
        max_n = n;
    }
    return rec[n];
}
