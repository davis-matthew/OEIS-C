/*
 * A295348  Number of nX4 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 2 or 3 1s.
 * https://oeis.org/A295348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays in which each 1 is adjacent *
 *            (horizontally or vertically) to either 0, 2, or 3 other 1‑cells
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pre‑computed transition tables: */
static int valid_table[16][16][16];  /* valid[up][row][down] */
static int final_table[16][16];      /* final[up][row]  (down = 0) */
static int tables_ready = 0;

/* Check if all 1‑cells in `row` have a number of 1‑neighbors in
 * {0,2,3}, given the 1‑neighbours above (`up`) and below (`down`). */
static inline int row_ok(unsigned int row, unsigned int up, unsigned int down)
{
    for (int j = 0; j < 4; ++j)
    {
        if ((row >> j) & 1U)
        {
            int cnt = 0;
            if (j > 0 && ((row >> (j - 1)) & 1U)) ++cnt;
            if (j + 1 < 4 && ((row >> (j + 1)) & 1U)) ++cnt;
            if ((up >> j) & 1U) ++cnt;
            if ((down >> j) & 1U) ++cnt;
            if (cnt == 1 || cnt == 4) return 0;          /* forbidden */
        }
    }
    return 1;                                           /* allowed */
}

seq_t a(idx_t n)
{
    /* Build transition tables once. */
    if (!tables_ready)
    {
        for (int up = 0; up < 16; ++up)
            for (int row = 0; row < 16; ++row)
                for (int down = 0; down < 16; ++down)
                    valid_table[up][row][down] = row_ok(row, up, down);
        for (int up = 0; up < 16; ++up)
            for (int row = 0; row < 16; ++row)
                final_table[up][row] = row_ok(row, up, 0);
        tables_ready = 1;
    }

    static seq_t dp[256], newdp[256];
    #define IDX(a,b)  (((a) << 4) | (b))

    /* initialise dp for depth 0 (no rows yet) */
    for (int i = 0; i < 256; ++i) dp[i] = 0;
    dp[0] = 1;                         /* state (prevprev=0 , prev=0) */

    for (idx_t r = 1; r <= n; ++r)
    {
        for (int i = 0; i < 256; ++i) newdp[i] = 0;
        for (int prevprev = 0; prevprev < 16; ++prevprev)
            for (int prev = 0; prev < 16; ++prev)
            {
                seq_t curcnt = dp[IDX(prevprev, prev)];
                if (!curcnt) continue;
                for (int curr = 0; curr < 16; ++curr)
                {
                    if (!valid_table[prevprev][prev][curr]) continue;
                    newdp[IDX(prev, curr)] += curcnt;
                }
            }
        /* swap dp and newdp */
        for (int i = 0; i < 256; ++i) dp[i] = newdp[i];
    }

    /* final check for the last row: no row below (down = 0) */
    seq_t total = 0;
    for (int prev = 0; prev < 16; ++prev)
        for (int curr = 0; curr < 16; ++curr)
        {
            seq_t curcnt = dp[IDX(prev, curr)];
            if (!curcnt) continue;
            if (!final_table[prev][curr]) continue;
            total += curcnt;
        }

    return total;
}
