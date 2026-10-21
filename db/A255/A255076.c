/*
 * A255076  Number of (n+2)X(2+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum one and no antidiagonal sum one.
 * https://oeis.org/A255076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts (n+2)×4 binary arrays with no 3×3 subblock whose
 *            main * diagonal sum equals 1 and no anti‑diagonal sum equals 1.
 *            * Computed by dynamic programming over rows: the state is the
 *            last two * rows (each a 4‑bit pattern), and a transition is
 *            allowed iff the new row * together with the previo...
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Number of possible row patterns (4 bits) */
    const idx_t ROWS = 16;
    /* State index: (prev2 << 4) | prev1 → 0..255 */
    const idx_t STATES = ROWS * ROWS;   /* 256 */

    /* DP arrays */
    seq_t cur[STATES];
    seq_t nxt[STATES];
    seq_t *pcur = cur, *pnxt = nxt;

    /* Initialize: any first two rows are admissible */
    for (idx_t i = 0; i < STATES; ++i)
        pcur[i] = (seq_t)0;
    for (idx_t a = 0; a < ROWS; ++a)
        for (idx_t b = 0; b < ROWS; ++b)
            pcur[(a << 4) | b] = (seq_t)1;

    /* For each additional row (there are n additional rows
       after the initial two) perform transitions */
    for (idx_t step = n; step > 0; --step)
    {
        /* clear next array */
        for (idx_t i = 0; i < STATES; ++i)
            pnxt[i] = (seq_t)0;

        /* iterate over all current states */
        for (idx_t a = 0; a < ROWS; ++a)
        {
            for (idx_t b = 0; b < ROWS; ++b)
            {
                seq_t val = pcur[(a << 4) | b];
                if (val == (seq_t)0)
                    continue;

                for (idx_t nc = 0; nc < ROWS; ++nc)
                {
                    int ok = 1;

                    /* Window starting at column 0 */
                    int sum;
                    sum = ((a   >> 0) & 1) + ((b   >> 1) & 1) + ((nc  >> 2) & 1);
                    if (sum == 1) ok = 0;
                    sum = ((a   >> 2) & 1) + ((b   >> 1) & 1) + ((nc  >> 0) & 1);
                    if (sum == 1) ok = 0;

                    /* Window starting at column 1 */
                    if (ok)
                    {
                        sum = ((a   >> 1) & 1) + ((b   >> 2) & 1) + ((nc  >> 3) & 1);
                        if (sum == 1) ok = 0;
                        sum = ((a   >> 3) & 1) + ((b   >> 2) & 1) + ((nc  >> 1) & 1);
                        if (sum == 1) ok = 0;
                    }

                    if (!ok)
                        continue;

                    /* Accept transition */
                    pnxt[(b << 4) | nc] += val;
                }
            }
        }

        /* swap pointers for next iteration */
        seq_t *tmp = pcur;
        pcur = pnxt;
        pnxt = tmp;
    }

    /* Sum all states to obtain the total number of valid arrays */
    seq_t ans = (seq_t)0;
    for (idx_t i = 0; i < STATES; ++i)
        ans += pcur[i];

    return ans;
}
