/*
 * A277940  Number of nX3 0..2 arrays with every element plus 1 mod 3 equal to
 *          some element at offset (-1,0) (-1,1) (0,-1) (0,1) or (1,0), with
 *          upper left element zero.
 * https://oeis.org/A277940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays over {0,1,2} with the rule that each
 *            element plus 1 mod 3 equals some neighbour among the offsets
 *            (-1,0), (-1,1), (0,-1), (0,1), or (1,0); the upper left element
 *            is 0. Computed by dynamic programming
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define STATE_COUNT 216   /* 6^3 possible (color,need) triples for 3 columns */

seq_t a(idx_t n)
{
    /* Safety: for n == 0 (not requested) return 0 */
    if (n == 0)
        return (seq_t)0;

    seq_t dp[2][STATE_COUNT];
    int cur = 0, nxt = 1;

    /* initialise all entries to 0 */
    for (int i = 0; i < STATE_COUNT; ++i)
        dp[cur][i] = (seq_t)0;

    /* ---------- initial row (row 0) ----------
       column 0 is fixed to 0 */
    for (int c1 = 0; c1 < 3; ++c1)
    {
        for (int c2 = 0; c2 < 3; ++c2)
        {
            int col[3] = {0, c1, c2};
            int need[3];

            for (int c = 0; c < 3; ++c)
            {
                int v = col[c];
                int sat = 0;

                /* left neighbour */
                if (c > 0 && col[c - 1] == (v + 1) % 3)
                    sat = 1;
                /* right neighbour */
                if (c < 2 && col[c + 1] == (v + 1) % 3)
                    sat = 1;

                need[c] = sat ? 0 : 1;
            }

            /* encode state: color*2 + need for each column */
            int cs0 = col[0] * 2 + need[0];
            int cs1 = col[1] * 2 + need[1];
            int cs2 = col[2] * 2 + need[2];
            int state = cs0 + 6 * cs1 + 36 * cs2;

            dp[cur][state] += (seq_t)1;
        }
    }

    /* ---------- subsequent rows ----------
       iterate over rows 1 .. n-1 (total n-1 additional rows) */
    for (idx_t r = 1; r < n; ++r)
    {
        /* reset next DP row to 0 */
        for (int i = 0; i < STATE_COUNT; ++i)
            dp[nxt][i] = (seq_t)0;

        /* iterate over all current states */
        for (int s = 0; s < STATE_COUNT; ++s)
        {
            seq_t ways = dp[cur][s];
            if (ways == (seq_t)0)
                continue;

            /* decode previous state's colour and need flags */
            int tmp = s;
            int cs0 = tmp % 6;  tmp /= 6;
            int colPrev0 = cs0 / 2;
            int needPrev0 = cs0 % 2;

            int cs1 = tmp % 6;  tmp /= 6;
            int colPrev1 = cs1 / 2;
            int needPrev1 = cs1 % 2;

            int cs2 = tmp % 6;  /* tmp /= 6 not needed */
            int colPrev2 = cs2 / 2;
            int needPrev2 = cs2 % 2;

            /* try all 27 colour triples for the current row */
            for (int a = 0; a < 3; ++a)
            {
                for (int b = 0; b < 3; ++b)
                {
                    for (int c = 0; c < 3; ++c)
                    {
                        /* check constraints from the upper row */
                        if (needPrev0 && a != (colPrev0 + 1) % 3)
                            continue;
                        if (needPrev1 && b != (colPrev1 + 1) % 3)
                            continue;
                        if (needPrev2 && c != (colPrev2 + 1) % 3)
                            continue;

                        int col_cur[3] = {a, b, c};
                        int need_cur[3];

                        /* compute need flags for current row */
                        for (int col = 0; col < 3; ++col)
                        {
                            int v = col_cur[col];
                            int sat = 0;

                            /* left neighbour */
                            if (col > 0 && col_cur[col - 1] == (v + 1) % 3)
                                sat = 1;
                            /* right neighbour */
                            if (col < 2 && col_cur[col + 1] == (v + 1) % 3)
                                sat = 1;
                            /* upper neighbour */
                            int prev_col = (col == 0) ? colPrev0 :
                                           (col == 1) ? colPrev1 : colPrev2;
                            if (prev_col == (v + 1) % 3)
                                sat = 1;
                            /* upper‑right neighbour */
                            if (col < 2)
                            {
                                int upRight = (col == 0) ? colPrev1 :
                                              (col == 1) ? colPrev2 : 0;
                                if (upRight == (v + 1) % 3)
                                    sat = 1;
                            }

                            need_cur[col] = sat ? 0 : 1;
                        }

                        /* encode new state */
                        int ncs0 = col_cur[0] * 2 + need_cur[0];
                        int ncs1 = col_cur[1] * 2 + need_cur[1];
                        int ncs2 = col_cur[2] * 2 + need_cur[2];
                        int new_state = ncs0 + 6 * ncs1 + 36 * ncs2;

                        dp[nxt][new_state] += ways;
                    }
                }
            }
        }

        /* prepare for next iteration */
        int tmpInt = cur;
        cur = nxt;
        nxt = tmpInt;
    }

    /* ---------- collect answer ----------
       sum all states where every need flag is 0 (satisfied) */
    seq_t result = (seq_t)0;
    for (int s = 0; s < STATE_COUNT; ++s)
    {
        int tmp = s;
        int need0 = (tmp % 6) % 2;  tmp /= 6;
        int need1 = (tmp % 6) % 2;  tmp /= 6;
        int need2 = (tmp % 6) % 2;
        if (need0 == 0 && need1 == 0 && need2 == 0)
            result += dp[cur][s];
    }

    return result;
}
