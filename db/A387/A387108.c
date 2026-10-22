/*
 * A387108  Number of entries in the n-th row of Pascal's triangle not
 *          divisible by 25.
 * https://oeis.org/A387108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of entries in the n-th row of Pascal's triangle
 *            not divisible by 25
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses Kummer's theorem: a binomial coefficient C(n,k) is not divisible by 25
   iff the addition k + (n-k) in base 5 produces at most one carry. */

seq_t a(idx_t n)
{
    /* Convert n to base 5 digits (least‑significant first) */
    idx_t tmp = n;
    unsigned int digits[64];
    int len = 0;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            digits[len++] = tmp % 5u;
            tmp /= 5u;
        }
    }

    /* Dynamic programming over digits:
       dp[carry_in][carries_used] = number of ways to reach this state
       carry_in   : 0 or 1 (carry going into the next higher digit)
       carries_used : 0 or 1 (total carries so far, we only keep paths ≤ 1) */
    seq_t dp[2][2] = {};
    dp[0][0] = 1;                /* start with no carry, no carries used */

    for (int pos = 0; pos < len; ++pos) {
        seq_t next[2][2] = {};

        unsigned int d = digits[pos];   /* target digit of n in this position */

        for (int carry_in = 0; carry_in <= 1; ++carry_in) {
            for (int used = 0; used <= 1; ++used) {
                seq_t val = dp[carry_in][used];
                if (!val) continue;

                /* Enumerate all possible digit pairs (a,b) for k and n-k */
                for (int a = 0; a < 5; ++a) {
                    for (int b = 0; b < 5; ++b) {
                        int sum = a + b + carry_in;
                        int carry_out = sum >= 5 ? 1 : 0;
                        int newcarry = sum % 5;
                        if (newcarry != d) continue;          /* must match n's digit */
                        if (used + carry_out > 1) continue;   /* only up to 1 carry allowed */

                        next[carry_out][used + carry_out] += val;
                    }
                }
            }
        }

        /* Prepare for next digit */
        for (int ci = 0; ci < 2; ++ci)
            for (int cu = 0; cu < 2; ++cu)
                dp[ci][cu] = next[ci][cu];
    }

    /* After processing all digits, the final carry must be zero.
       Sum all paths that ended with carry_in = 0. */
    return dp[0][0] + dp[0][1];
}
