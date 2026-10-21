/*
 * A298956  Number of n X 7 0..1 arrays with every element equal to 3, 4, 5, 7
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of n×7 binary arrays with upper‑left element 0 such that
 *  each cell has 3,4,5,7 or 8 equal king‑move adjacent elements (OEIS A298956).
 *  Computed by dynamic programming over pairs of consecutive rows.
 */
#include <stdint.h>
#include <stdbool.h>

/* sentinel value for an absent row */
#define SENTINEL 128U

/* pre‑computed adjacency: allowed_next[prev][curr] gives a two‑word
 * bitmask (64 bits each) of next rows that make the middle row 'curr'
 * satisfy its constraints with neighbors 'prev' and 'next'. */
static uint64_t allowed_low[129][128];
static uint64_t allowed_high[129][128];
static bool allowed_init = false;

/* check whether a row 'curr' satisfies its constraints given
 * its above neighbor row 'prev' and below neighbor row 'next'.
 * If a neighbor row is SENTINEL it is treated as absent.  */
static inline bool row_valid(unsigned prev, unsigned curr, unsigned next)
{
    for (int j = 0; j < 7; ++j) {
        unsigned val = (curr >> j) & 1U;
        unsigned cnt = 0;

        /* left */
        if (j > 0 && val == ((curr >> (j - 1)) & 1U)) ++cnt;
        /* right */
        if (j < 6 && val == ((curr >> (j + 1)) & 1U)) ++cnt;

        /* above */
        if (prev != SENTINEL) {
            if (j > 0   && val == ((prev >> (j - 1)) & 1U)) ++cnt;
            if (val == ((prev >> j) & 1U)) ++cnt;
            if (j < 6   && val == ((prev >> (j + 1)) & 1U)) ++cnt;
        }
        /* below */
        if (next != SENTINEL) {
            if (j > 0   && val == ((next >> (j - 1)) & 1U)) ++cnt;
            if (val == ((next >> j) & 1U)) ++cnt;
            if (j < 6   && val == ((next >> (j + 1)) & 1U)) ++cnt;
        }

        if (!(cnt == 3 || cnt == 4 || cnt == 5 || cnt == 7 || cnt == 8))
            return false;
    }
    return true;
}

/* initialize the adjacency tables once */
static void init_allowed(void)
{
    if (allowed_init) return;
    for (unsigned prev = 0; prev <= SENTINEL; ++prev) {
        for (unsigned curr = 0; curr < 128; ++curr) {
            uint64_t low = 0, high = 0;
            for (unsigned next = 0; next < 128; ++next) {
                if (row_valid(prev, curr, next)) {
                    if (next < 64)
                        low  |= (uint64_t)1 << next;
                    else
                        high |= (uint64_t)1 << (next - 64);
                }
            }
            allowed_low[prev][curr]  = low;
            allowed_high[prev][curr] = high;
        }
    }
    allowed_init = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init_allowed();

    /* 1×7 case: count patterns that satisfy constraints with no neighbours */
    if (n == 1) {
        seq_t total = 0;
        for (unsigned pat = 0; pat < 128; ++pat) {
            if (row_valid(SENTINEL, pat, SENTINEL))
                total += (seq_t)1;
        }
        return total;
    }

    /* Dynamic programming over first two rows */
    static seq_t dp[2][128][128];
    int cur = 0, nxt = 1;

    /* clear dp arrays */
    for (int i = 0; i < 128; ++i)
        for (int j = 0; j < 128; ++j) {
            dp[cur][i][j] = 0;
            dp[nxt][i][j] = 0;
        }

    /* initialize with sentinel previous row */
    for (unsigned r0 = 0; r0 < 128; ++r0) {
        if ((r0 & 1) != 0) continue;          /* upper‑left must be 0 */
        for (unsigned r1 = 0; r1 < 128; ++r1) {
            if (row_valid(SENTINEL, r0, r1))
                dp[cur][r0][r1] = (seq_t)1;
        }
    }

    /* iterate over remaining rows */
    for (idx_t row = 2; row < n; ++row) {
        /* clear next array */
        for (int i = 0; i < 128; ++i)
            for (int j = 0; j < 128; ++j)
                dp[nxt][i][j] = 0;

        for (unsigned prev = 0; prev < 128; ++prev) {
            for (unsigned curr = 0; curr < 128; ++curr) {
                seq_t cnt = dp[cur][prev][curr];
                if (cnt == 0) continue;

                uint64_t low = allowed_low[prev][curr];
                uint64_t high = allowed_high[prev][curr];

                while (low) {
                    unsigned next = __builtin_ctzll(low);
                    low &= low - 1;
                    dp[nxt][curr][next] += cnt;
                }
                while (high) {
                    unsigned next = __builtin_ctzll(high) + 64;
                    high &= high - 1;
                    dp[nxt][curr][next] += cnt;
                }
            }
        }
        /* swap */
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* final answer: check last row constraints with sentinel below */
    seq_t total = 0;
    for (unsigned prev = 0; prev < 128; ++prev) {
        for (unsigned curr = 0; curr < 128; ++curr) {
            seq_t cnt = dp[cur][prev][curr];
            if (cnt == 0) continue;
            if (row_valid(prev, curr, SENTINEL))
                total += cnt;
        }
    }
    return total;
}
