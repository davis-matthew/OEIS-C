/*
 * A298572  Number of nX5 0..1 arrays with every element equal to 0, 2, 4, 5
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 0..1 arrays where each element has a number
 *            of king‑move adjacent equal elements in {0,2,4,5,7}, with upper
 *            left element zero. (OEIS A298572)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* allowed number of equal neighbours */
static const unsigned char allowed_counts[9] = {
    1, /* 0 */
    0, /* 1 */
    1, /* 2 */
    0, /* 3 */
    1, /* 4 */
    1, /* 5 */
    0, /* 6 */
    1, /* 7 */
    0  /* 8 */
};

/* tables for transition masks
   first_next_mask[cur]     : bit nxt set if cur (first row) is valid with next row nxt
   middle_mask[prev][cur]   : bit nxt set if cur is valid with prev and nxt
   last_mask[prev][cur]     : bit 0 set if cur is valid with prev and no lower row   */
static unsigned first_next_mask[32];
static unsigned middle_mask[32][32];
static unsigned last_mask[32][32];

/* check whether cur row is valid when its upper neighbours exist or not
   and its lower neighbours exist or not */
static int is_row_valid(uint32_t prevRow, uint32_t curRow, uint32_t nextRow,
                        int hasPrev, int hasNext)
{
    for (int j = 0; j < 5; ++j) {
        int val = (curRow >> j) & 1u;
        int cnt = 0;

        /* same row neighbours */
        if (j > 0 && ((curRow >> (j - 1)) & 1u) == val) cnt++;
        if (j < 4 && ((curRow >> (j + 1)) & 1u) == val) cnt++;

        /* upper row neighbours */
        if (hasPrev) {
            if (j > 0 && ((prevRow >> (j - 1)) & 1u) == val) cnt++;
            if (((prevRow >> j) & 1u) == val) cnt++;
            if (j < 4 && ((prevRow >> (j + 1)) & 1u) == val) cnt++;
        }

        /* lower row neighbours */
        if (hasNext) {
            if (j > 0 && ((nextRow >> (j - 1)) & 1u) == val) cnt++;
            if (((nextRow >> j) & 1u) == val) cnt++;
            if (j < 4 && ((nextRow >> (j + 1)) & 1u) == val) cnt++;
        }

        if (!allowed_counts[cnt]) return 0;
    }
    return 1;
}

/* initialise transition tables – executed once */
static void init_tables(void)
{
    for (uint32_t cur = 0; cur < 32; ++cur) {
        unsigned mask = 0;
        for (uint32_t nxt = 0; nxt < 32; ++nxt) {
            if (is_row_valid(0, cur, nxt, 0, 1))   /* no upper row, lower row exists */
                mask |= 1u << nxt;
        }
        first_next_mask[cur] = mask;
    }

    for (uint32_t prev = 0; prev < 32; ++prev) {
        for (uint32_t cur = 0; cur < 32; ++cur) {
            unsigned mask = 0;
            for (uint32_t nxt = 0; nxt < 32; ++nxt) {
                if (is_row_valid(prev, cur, nxt, 1, 1))
                    mask |= 1u << nxt;
            }
            middle_mask[prev][cur] = mask;
        }
    }

    for (uint32_t prev = 0; prev < 32; ++prev) {
        for (uint32_t cur = 0; cur < 32; ++cur) {
            unsigned mask = 0;
            if (is_row_valid(prev, cur, 0, 1, 0))   /* no lower row */
                mask |= 1u << 0;
            last_mask[prev][cur] = mask;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    static int started = 0;
    if (!started) { init_tables(); started = 1; }

    if (n == 0)  /* offset is 1; values for n<1 are undefined */
        return 0;

    /* for n==1 we only have one row */
    if (n == 1) {
        seq_t res = 0;
        for (uint32_t row = 0; row < 32; ++row) {
            if ((row & 1u) == 0 &&                /* upper left zero */
                is_row_valid(0, row, 0, 0, 0)) {   /* no neighbours at all */
                res += (seq_t)1;
            }
        }
        return res;
    }

    static seq_t dp[32][32];
    static seq_t dp_next[32][32];

    /* clear dp */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    /* initial transition: choose first and second rows */
    for (uint32_t first = 0; first < 32; ++first) {
        if ((first & 1u) == 0) {                     /* upper left zero */
            unsigned mask = first_next_mask[first];
            for (uint32_t nxt = 0; nxt < 32; ++nxt) {
                if (mask & (1u << nxt))
                    dp[first][nxt] += (seq_t)1;
            }
        }
    }

    idx_t steps = n - 2;   /* remaining rows after the first two */
    for (idx_t step = 0; step < steps; ++step) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_next[i][j] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int cur = 0; cur < 32; ++cur) {
                seq_t ways = dp[prev][cur];
                if (ways == 0) continue;
                unsigned mask = middle_mask[prev][cur];
                for (int nxt = 0; nxt < 32; ++nxt) {
                    if (mask & (1u << nxt))
                        dp_next[cur][nxt] += ways;
                }
            }
        }

        /* copy dp_next -> dp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* final row: must be valid with its upper neighbour and no lower neighbour */
    seq_t result = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int cur = 0; cur < 32; ++cur) {
            seq_t ways = dp[prev][cur];
            if (ways == 0) continue;
            if (last_mask[prev][cur] & 1u)
                result += ways;
        }
    }
    return result;
}
