/*
 * A242632  Number of n-length words w over a 9-ary alphabet {a_1,...,a_9}
 *          such that w contains never more than j consecutive letters a_j
 *          (for 1<=j<=9).
 * https://oeis.org/A242632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A242632: Number of n‑length words over a 9‑ary alphabet
   with at most j consecutive a_j (j = 1..9). This DP counts all
   allowed words by tracking the last letter and the current run length. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;          /* a(0) = 1 */

    const int letters = 9;                /* a_1 … a_9  */
    seq_t cur[9][10] = {0};
    seq_t nxt[9][10] = {0};

    /* initial words of length 1 */
    for (int i = 0; i < letters; ++i)
        cur[i][1] = 1;   /* run length 1 for each letter */

    /* build words of length 2 … n */
    for (idx_t len = 2; len <= n; ++len) {
        /* clear next array */
        for (int i = 0; i < letters; ++i)
            for (int r = 1; r <= i + 1; ++r)
                nxt[i][r] = 0;

        /* transition from cur to nxt */
        for (int i = 0; i < letters; ++i) {
            int limit = i + 1;                  /* maximum run length for letter i */
            for (int r = 1; r <= limit; ++r) {
                seq_t val = cur[i][r];
                if (!val) continue;

                /* append same letter if run length not exceeded */
                if (r < limit)
                    nxt[i][r + 1] += val;

                /* append any different letter */
                for (int j = 0; j < letters; ++j) {
                    if (j == i) continue;
                    nxt[j][1] += val;
                }
            }
        }

        /* copy nxt to cur for next iteration */
        for (int i = 0; i < letters; ++i)
            for (int r = 1; r <= i + 1; ++r)
                cur[i][r] = nxt[i][r];
    }

    /* sum all states to obtain a(n) */
    seq_t total = 0;
    for (int i = 0; i < letters; ++i)
        for (int r = 1; r <= i + 1; ++r)
            total += cur[i][r];

    return total;
}
