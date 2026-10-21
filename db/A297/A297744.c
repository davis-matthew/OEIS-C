/*
 * A297744  Number of n X 3 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 1, 3 or 4 neighboring 1's.
 * https://oeis.org/A297744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 binary matrices in which each 1 has 1, 3,
 *            or 4 neighboring 1s horizontally, diagonally or antidiagonally.
 *            (The OEIS recurrence of order 14 is a(n) =
 *            3*a(n-1)+46*a(n-3)-97*a(n-4)-5*a(n-5)-406*a(n-6)+458*a(n-7)
 *            -219*a(n-8)+686*a(n-9)-458*a(n-10)+566*a(n-11)+308*a(n-12)
 *            -860*a(n...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool valid_trip[512];
static bool valid_trip_initialized = false;

static inline bool valid_row(uint8_t prev, uint8_t cur, uint8_t next)
{
    for (int pos = 0; pos < 3; ++pos) {
        if (cur & (1u << pos)) {
            int cnt = 0;
            /* same row horizontally */
            if (pos > 0 && (cur & (1u << (pos - 1)))) ++cnt;
            if (pos < 2 && (cur & (1u << (pos + 1)))) ++cnt;
            /* previous row diagonally */
            if (pos > 0 && (prev & (1u << (pos - 1)))) ++cnt;
            if (pos < 2 && (prev & (1u << (pos + 1)))) ++cnt;
            /* next row diagonally */
            if (pos > 0 && (next & (1u << (pos - 1)))) ++cnt;
            if (pos < 2 && (next & (1u << (pos + 1)))) ++cnt;
            if (cnt != 1 && cnt != 3 && cnt != 4) return false;
        }
    }
    return true;
}

static void init_valid_trip(void)
{
    for (int prev = 0; prev < 8; ++prev)
        for (int cur = 0; cur < 8; ++cur)
            for (int next = 0; next < 8; ++next) {
                int idx = (prev << 6) | (cur << 3) | next;
                valid_trip[idx] = valid_row((uint8_t)prev,
                                            (uint8_t)cur,
                                            (uint8_t)next);
            }
    valid_trip_initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (!valid_trip_initialized) init_valid_trip();

    /* n = 1 ---------------------------------------------------------- */
    if (n == 1) {
        seq_t count = (seq_t)0;
        for (int cur = 0; cur < 8; ++cur)
            if (valid_trip[(0 << 6) | (cur << 3) | 0]) ++count;
        return count;
    }

    /* DP tables for last two rows ----------------------------------- */
    seq_t dp[8][8];
    seq_t next_dp[8][8];
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            dp[i][j] = (seq_t)0;

    /* initial two rows ---------------------------------------------- */
    for (int r1 = 0; r1 < 8; ++r1)
        for (int r2 = 0; r2 < 8; ++r2)
            if (valid_trip[(0 << 6) | (r1 << 3) | r2])
                dp[r1][r2] = (seq_t)1;

    /* n = 2 ---------------------------------------------------------- */
    if (n == 2) {
        seq_t ans = (seq_t)0;
        for (int r1 = 0; r1 < 8; ++r1)
            for (int r2 = 0; r2 < 8; ++r2)
                if (dp[r1][r2] &&
                    valid_trip[(r1 << 6) | (r2 << 3) | 0])
                    ans += dp[r1][r2];
        return ans;
    }

    /* general case n ≥ 3 --------------------------------------------- */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                next_dp[i][j] = (seq_t)0;

        for (int r1 = 0; r1 < 8; ++r1)
            for (int r2 = 0; r2 < 8; ++r2) {
                seq_t ways = dp[r1][r2];
                if (!ways) continue;
                for (int r3 = 0; r3 < 8; ++r3)
                    if (valid_trip[(r1 << 6) | (r2 << 3) | r3])
                        next_dp[r2][r3] += ways;
            }

        /* copy next_dp back to dp ------------------------------------- */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = next_dp[i][j];
    }

    /* final check for last row (next = 0) ----------------------------- */
    seq_t ans = (seq_t)0;
    for (int r1 = 0; r1 < 8; ++r1)
        for (int r2 = 0; r2 < 8; ++r2) {
            seq_t ways = dp[r1][r2];
            if (!ways) continue;
            if (valid_trip[(r1 << 6) | (r2 << 3) | 0])
                ans += ways;
        }
    return ans;
}
