/*
 * A241930  Number of length n+4 0..2 arrays with no consecutive five elements
 *          summing to more than 2*2.
 * https://oeis.org/A241930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP counting of (n+4)-length arrays over {0,1,2} with no 5
 *            consecutive elements summing to more than 4
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 22, u64 n <= 49, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 3^4 possible 4‑tuples of last four elements. */
#define STATES 81

/* Return an index (0..80) for a 4‑tuple. */
static int encode_state(int a, int b, int c, int d)
{
    return a * 27 + b * 9 + c * 3 + d;
}

seq_t a(idx_t n)
{
    /* Length of the array to be considered. */
    idx_t L = n + 4;

    /* dp[parity][state] holds counts of sequences of the current
       length ending with the given 4‑tuple. */
    seq_t dp[2][STATES];

    /* Initialise dp arrays to zero. */
    for (int p = 0; p < 2; ++p)
        for (int s = 0; s < STATES; ++s)
            dp[p][s] = 0;

    /* Start with the empty array: state (0,0,0,0). */
    dp[0][0] = 1;

    /* Pre‑decode state values for quick transitions. */
    int a_val[STATES], b_val[STATES], c_val[STATES], d_val[STATES];
    for (int s = 0; s < STATES; ++s) {
        a_val[s] = s / 27;
        b_val[s] = (s / 9) % 3;
        c_val[s] = (s / 3) % 3;
        d_val[s] = s % 3;
    }

    int cur = 0, nxt = 1;

    /* Main DP loop: add one element at each step. */
    for (idx_t len = 1; len <= L; ++len) {
        /* Clear next row. */
        for (int s = 0; s < STATES; ++s)
            dp[nxt][s] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t ways = dp[cur][s];
            if (!ways)
                continue;

            int a0 = a_val[s], b0 = b_val[s];
            int c0 = c_val[s], d0 = d_val[s];

            for (int x = 0; x <= 2; ++x) {
                /* Constraint applies only when a full window of 5 has been
                   formed. */
                if (len >= 5 && (a0 + b0 + c0 + d0 + x) > 4)
                    continue;

                int t = encode_state(b0, c0, d0, x);
                dp[nxt][t] = dp[nxt][t] + ways;
            }
        }
        /* Swap the roles of the two rows for the next iteration. */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum over all final states to obtain the total number of sequences. */
    seq_t result = 0;
    for (int s = 0; s < STATES; ++s)
        result = result + dp[cur][s];

    return result;
}
