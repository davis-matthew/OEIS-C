/*
 * A269653  Number of length-n 0..5 arrays with no adjacent pair x,x+1
 *          repeated.
 * https://oeis.org/A269653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑n arrays over {0,…,5} in which no
 *            adjacent pair x,x+1 occurs more than once. Offset 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)6;                     /* a(1) = 6 */

    const int ALPHABET = 6;                   /* symbols 0–5 */
    const int PAIRS   = 5;                    /* rising pairs 0→1,…,4→5 */
    const int MASKS   = 1 << PAIRS;           /* 32 masks */

    seq_t dp[MASKS][ALPHABET];
    seq_t next[MASKS][ALPHABET];

    /* initialise for length 1: no pair used, any last symbol */
    for (int m = 0; m < MASKS; ++m)
        for (int v = 0; v < ALPHABET; ++v)
            dp[m][v] = (seq_t)0;
    for (int v = 0; v < ALPHABET; ++v)
        dp[0][v] = (seq_t)1;

    for (idx_t pos = 1; pos < n; ++pos) {
        /* zero next layer */
        for (int m = 0; m < MASKS; ++m)
            for (int v = 0; v < ALPHABET; ++v)
                next[m][v] = (seq_t)0;

        /* dynamic programming step */
        for (int mask = 0; mask < MASKS; ++mask) {
            for (int prev = 0; prev < ALPHABET; ++prev) {
                seq_t cnt = dp[mask][prev];
                if (cnt == (seq_t)0) continue;
                for (int cur = 0; cur < ALPHABET; ++cur) {
                    int newmask = mask;
                    if (cur == prev + 1) {           /* ascending pair */
                        int bit = cur - 1;            /* 0–4 */
                        if (mask & (1 << bit)) continue; /* pair already used */
                        newmask |= 1 << bit;
                    }
                    next[newmask][cur] += cnt;
                }
            }
        }

        /* copy next into dp for the next iteration */
        for (int m = 0; m < MASKS; ++m)
            for (int v = 0; v < ALPHABET; ++v)
                dp[m][v] = next[m][v];
    }

    /* sum all states of length n */
    seq_t result = (seq_t)0;
    for (int mask = 0; mask < MASKS; ++mask)
        for (int v = 0; v < ALPHABET; ++v)
            result += dp[mask][v];

    return result;
}
