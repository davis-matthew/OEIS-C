/*
 * A241615  Number of length n+2 0..9 arrays with no consecutive three
 *          elements summing to more than 9.
 * https://oeis.org/A241615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+2 0..9 arrays with no consecutive
 *            three elements summing to more than 9
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1) {
        return 0;           /* the sequence is defined only for n ≥ 1 */
    }

    /* target length for the array */
    idx_t len_target = n + 2;

    /* DP layers: dp[layer][last_digit][second_last_digit] */
    seq_t dp[2][10][10];

    /* base case: arrays of length 2 – each pair of digits occurs once */
    for (int d1 = 0; d1 < 10; ++d1)
        for (int d2 = 0; d2 < 10; ++d2)
            dp[0][d1][d2] = 1;

    /* if the required length is 2, just return 100 */
    if (len_target == 2) {
        seq_t sum = 0;
        for (int d1 = 0; d1 < 10; ++d1)
            for (int d2 = 0; d2 < 10; ++d2)
                sum += dp[0][d1][d2];
        return sum;          /* 100 */
    }

    /* iterate over lengths from 3 up to the target length */
    for (idx_t len = 3; len <= len_target; ++len) {
        int cur  = len & 1;
        int prev = 1 - cur;

        /* clear current layer */
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                dp[cur][i][j] = 0;

        /* transition: add a new digit x to the existing suffix d1,d2  */
        for (int d1 = 0; d1 < 10; ++d1) {
            for (int d2 = 0; d2 < 10; ++d2) {
                seq_t val = dp[prev][d1][d2];
                if (!val) continue;
                int maxx = 9 - (d1 + d2);
                if (maxx < 0) continue;
                for (int x = 0; x <= maxx; ++x) {
                    dp[cur][d2][x] += val;
                }
            }
        }
    }

    /* sum all states of the final layer to get the answer */
    int final_layer = len_target & 1;
    seq_t result = 0;
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            result += dp[final_layer][i][j];

    return result;
}
