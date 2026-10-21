/*
 * A283836  Number of length-n binary vectors beginning with 0, ending with 1,
 *          and avoiding 6 consecutive 0's and 6 consecutive 1's.
 * https://oeis.org/A283836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of length‑n binary vectors beginning with 0, ending with 1,
   and avoiding 6 consecutive 0's and 6 consecutive 1's. */

seq_t a(idx_t n)
{
    if (n == 0)                return (seq_t)1;   // a(0) = 1
    if (n == 1)                return (seq_t)0;   // a(1) = 0

    seq_t cur[2][5];
    seq_t nxt[2][5];

    /* initialize to zero */
    for (int b = 0; b < 2; ++b)
        for (int r = 0; r < 5; ++r)
            cur[b][r] = nxt[b][r] = (seq_t)0;

    /* start with a single 0 */
    cur[0][0] = (seq_t)1;

    /* iterate over the remaining n-1 positions */
    for (idx_t pos = 1; pos < n; ++pos)
    {
        /* zero next state */
        for (int b = 0; b < 2; ++b)
            for (int r = 0; r < 5; ++r)
                nxt[b][r] = (seq_t)0;

        /* transition */
        for (int b = 0; b < 2; ++b)
            for (int r = 0; r < 5; ++r)
            {
                seq_t v = cur[b][r];
                if (v == (seq_t)0) continue;

                /* same bit, extend run if < 5 */
                if (r < 4) nxt[b][r + 1] += v;

                /* flip bit, reset run to 1 */
                nxt[1 - b][0] += v;
            }

        /* swap cur and nxt */
        for (int b = 0; b < 2; ++b)
            for (int r = 0; r < 5; ++r)
                cur[b][r] = nxt[b][r];
    }

    /* sum states that end with a 1 */
    seq_t res = (seq_t)0;
    for (int r = 0; r < 5; ++r)
        res += cur[1][r];

    return res;
}
