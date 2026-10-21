/*
 * A299077  Number of nX4 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4 or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of n×4 0/1 arrays with the upper‑left *
 *            element fixed to 0, such that for every cell the number of king‑
 *            * move adjacent cells of the same value is 0,1,2,3,4 or 7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
/*  Precomputation of validity tables                                    */
#include <stdint.h>
#include <stdbool.h>

/*  Allowed neighbour counts for a cell (0–8). */
static const bool allowed[9] = {
    true,  /* 0 */
    true,  /* 1 */
    true,  /* 2 */
    true,  /* 3 */
    true,  /* 4 */
    false, /* 5 */
    false, /* 6 */
    true,  /* 7 */
    false  /* 8 */
};

/*  Check whether a row satisfies the condition, given which
 *  neighbours are present and the actual rows above/below. */
static bool row_ok(int row, bool above_exists, bool below_exists,
                   int above_row, int below_row)
{
    for (int col = 0; col < 4; ++col) {
        int cur   = (row >> col) & 1;
        int cnt   = 0;

        /* same row */
        if (col > 0     && ((row >> (col - 1)) & 1) == cur) ++cnt;
        if (col < 3     && ((row >> (col + 1)) & 1) == cur) ++cnt;

        /* above row */
        if (above_exists) {
            if        (((above_row >> col) & 1) == cur) ++cnt;
            if (col>0   && ((above_row >> (col-1)) & 1) == cur) ++cnt;
            if (col<3   && ((above_row >> (col+1)) & 1) == cur) ++cnt;
        }

        /* below row */
        if (below_exists) {
            if        (((below_row >> col) & 1) == cur) ++cnt;
            if (col>0   && ((below_row >> (col-1)) & 1) == cur) ++cnt;
            if (col<3   && ((below_row >> (col+1)) & 1) == cur) ++cnt;
        }

        if (!allowed[cnt]) return false;
    }
    return true;
}

/*  Validity tables.  They are computed once on first call.  */
static bool valid_first[16][16];  /* first row given second row */
static bool valid_full[16][16][16];  /* middle row given prev and next rows */
static bool valid_last [16][16];  /* last row given previous row */
static bool valid_single[16];   /* only one row (no neighbours) */
static bool tables_ready = false;

static void init_tables(void)
{
    if (tables_ready) return;
    tables_ready = true;

    for (int r1 = 0; r1 < 16; ++r1) {
        for (int r2 = 0; r2 < 16; ++r2) {
            valid_first[r1][r2] = row_ok(r1, false, true, 0, r2);
            valid_last [r1][r2] = row_ok(r2, true,  false, r1, 0);
        }
    }

    for (int rp = 0; rp < 16; ++rp)
        for (int rc = 0; rc < 16; ++rc)
            for (int rn = 0; rn < 16; ++rn)
                valid_full[rp][rc][rn] = row_ok(rc, true, true, rp, rn);

    for (int r = 0; r < 16; ++r)
        valid_single[r] = row_ok(r, false, false, 0, 0);
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                               */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */

    init_tables();

    /* ------------  n == 1  ---------------------------------------- */
    if (n == 1) {
        seq_t ans = 0;
        for (int r = 0; r < 16; ++r)
            if ( (r & 1) == 0 && valid_single[r] )
                ++ans;
        return ans;
    }

    /* --------------  n >= 2  --------------------------------------- */
    /* dp[prev][cur] – number of ways to build rows up to cur,
     *   where all rows before prev are already satisfied.
     */
    seq_t dp[16][16] = {0};

    /* initialise with rows 1 and 2 */
    for (int r1 = 0; r1 < 16; ++r1)
        if ( (r1 & 1) == 0 )          /* upper‑left is 0 */
            for (int r2 = 0; r2 < 16; ++r2)
                if (valid_first[r1][r2])
                    dp[r1][r2] = 1;

    /* rows 3 … n  */
    for (idx_t row = 3; row <= n; ++row) {
        seq_t ndp[16][16] = {0};
        for (int prev = 0; prev < 16; ++prev)
            for (int cur  = 0; cur  < 16; ++cur)
                if (dp[prev][cur] != 0)
                    for (int nxt = 0; nxt < 16; ++nxt)
                        if (valid_full[prev][cur][nxt])
                            ndp[cur][nxt] += dp[prev][cur];
        for (int i = 0; i < 256; ++i)
            ((seq_t*)dp)[i] = ((seq_t*)ndp)[i];
    }

    /* --------------  final row check  ------------------------------ */
    seq_t ans = 0;
    for (int prev = 0; prev < 16; ++prev)
        for (int cur  = 0; cur  < 16; ++cur)
            if (dp[prev][cur] != 0 && valid_last[prev][cur])
                ans += dp[prev][cur];

    return ans;
}
