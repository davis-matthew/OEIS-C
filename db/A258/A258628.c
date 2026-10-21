/*
 * A258628  Number of length n 1..(5+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5 or 7.
 * https://oeis.org/A258628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts 1..6 arrays of length n whose every * leading
 *            partial sum is divisible by 2, 3, 5 or 7
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MOD 210
#define VALS 6

seq_t a(idx_t n)
{
    static seq_t dp[MOD];
    static seq_t next[MOD];

    /* initialise for length 0 */
    for (idx_t i = 0; i < MOD; ++i) dp[i] = 0;
    dp[0] = 1;

    for (idx_t step = 1; step <= n; ++step) {
        for (idx_t i = 0; i < MOD; ++i) next[i] = 0;
        for (idx_t r = 0; r < MOD; ++r) {
            seq_t cur = dp[r];
            if (!cur) continue;
            for (idx_t v = 1; v <= VALS; ++v) {
                idx_t nr = r + v;
                if (nr >= MOD) nr -= MOD;
                if ((nr % 2ULL == 0ULL) ||
                    (nr % 3ULL == 0ULL) ||
                    (nr % 5ULL == 0ULL) ||
                    (nr % 7ULL == 0ULL)) {
                    next[nr] += cur;
                }
            }
        }
        for (idx_t i = 0; i < MOD; ++i) dp[i] = next[i];
    }

    seq_t result = 0;
    for (idx_t i = 0; i < MOD; ++i) result += dp[i];
    return result;
}
