/*
 * A298083  Number of nX4 0..1 arrays with every element equal to 2, 3, 5 or 6
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A298083: Number of n×4 0/1 arrays with upper-left cell 0 such that
 * for every cell the number of king‑move adjacent cells equal to itself is
 * 2, 3, 5 or 6.
 * Computed by dynamic programming over rows with explicit edge handling.
 */

#include <stdint.h>
#include <stdbool.h>

/* internal constants */
static const int NONE = 16;          /* index meaning "no adjacent row" */
static const int COLS = 4;          /* width of the array */

/* pre‑computed validity tables:
 *   valid[prev][cur][next]    : cur row is valid with neighbours prev, next
 *   allowed_next[prev][cur]   : bitmask of next rows that keep cur valid
 *   valid_last[prev][cur]     : cur is valid when next is absent
 */
static bool initialized = false;
static bool valid[17][16][17];
static uint16_t allowed_next[17][16];
static bool valid_last[17][16];

/* helper: check whether cur row is valid given neighbours prev and next */
static inline bool check_valid(int prev, int cur, int next)
{
    for (int j = 0; j < COLS; ++j) {
        unsigned int cur_bit = (cur >> j) & 1u;
        unsigned int cnt = 0;

        /* neighbours in the previous row */
        if (prev != NONE) {
            if (j > 0 && ((prev >> (j - 1)) & 1u) == cur_bit) ++cnt;
            if (((prev >> j) & 1u) == cur_bit) ++cnt;
            if (j < COLS - 1 && ((prev >> (j + 1)) & 1u) == cur_bit) ++cnt;
        }

        /* neighbours in the current row (left/right) */
        if (j > 0 && ((cur >> (j - 1)) & 1u) == cur_bit) ++cnt;
        if (j < COLS - 1 && ((cur >> (j + 1)) & 1u) == cur_bit) ++cnt;

        /* neighbours in the next row */
        if (next != NONE) {
            if (j > 0 && ((next >> (j - 1)) & 1u) == cur_bit) ++cnt;
            if (((next >> j) & 1u) == cur_bit) ++cnt;
            if (j < COLS - 1 && ((next >> (j + 1)) & 1u) == cur_bit) ++cnt;
        }

        if (cnt != 2 && cnt != 3 && cnt != 5 && cnt != 6) return false;
    }
    return true;
}

/* initialize the validity tables once */
static void init_tables(void)
{
    if (initialized) return;
    for (int prev = 0; prev <= NONE; ++prev) {
        for (int cur = 0; cur < (1 << COLS); ++cur) {
            uint16_t mask = 0;
            for (int next = 0; next <= NONE; ++next) {
                if (check_valid(prev, cur, next)) {
                    valid[prev][cur][next] = true;
                    if (next != NONE) mask |= (1U << next);
                    if (next == NONE) valid_last[prev][cur] = true;
                } else {
                    valid[prev][cur][next] = false;
                }
            }
            allowed_next[prev][cur] = mask;
        }
    }
    initialized = true;
}

/* required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    init_tables();

    /* dp[prev][cur] counts ways up to the current row, with prev and cur indices */
    seq_t dp[17][16]      = {0};
    seq_t newdp[17][16]   = {0};

    /* first row: prev is NONE, cur can be any 4‑bit pattern with upper-left 0 */
    for (int cur = 0; cur < (1 << COLS); ++cur) {
        if ((cur & 1) == 0) dp[NONE][cur] = 1;
    }

    /* process rows 1 … n‑1 */
    for (idx_t row = 1; row < n; ++row) {
        for (int p = 0; p <= NONE; ++p)
            for (int c = 0; c < (1 << COLS); ++c)
                newdp[p][c] = 0;

        for (int prev = 0; prev <= NONE; ++prev) {
            for (int cur = 0; cur < (1 << COLS); ++cur) {
                seq_t ways = dp[prev][cur];
                if (!ways) continue;
                uint16_t mask = allowed_next[prev][cur];
                if (!mask) continue;
                for (int nxt = 0; nxt < (1 << COLS); ++nxt) {
                    if (mask & (1U << nxt)) {
                        newdp[cur][nxt] += ways;
                    }
                }
            }
        }
        /* swap dp and newdp */
        for (int p = 0; p <= NONE; ++p)
            for (int c = 0; c < (1 << COLS); ++c)
                dp[p][c] = newdp[p][c];
    }

    /* finalize: last row must be valid with next = NONE */
    seq_t ans = 0;
    for (int prev = 0; prev <= NONE; ++prev) {
        for (int cur = 0; cur < (1 << COLS); ++cur) {
            seq_t ways = dp[prev][cur];
            if (!ways) continue;
            if (valid_last[prev][cur]) ans += ways;
        }
    }
    return ans;
}
