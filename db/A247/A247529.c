/*
 * A247529  Number of length n+3 0..4 arrays with some disjoint pairs in every
 *          consecutive four terms having the same sum.
 * https://oeis.org/A247529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 27, u64 n <= 62, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by dynamic programming over the last three symbols.
   For each length L≥4, the DP state is the last 3 symbols of the
   partial array.  A new symbol is appended only if the resulting
   window of four symbols contains a partition into two equal‑sum pairs.
   The number of complete arrays of length n+3 is the sum of all DP
   states after L = n+3.  This runs in O(n·5³) time and uses O(5³)
   memory. */
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* index 0 is outside the offset */

    const idx_t L = n + 3;            /* target length of the array */

    /* Pre‑compute which quadruples satisfy the equal‑sum pair condition */
    static bool good[5][5][5][5];
    static bool init = false;
    if (!init) {
        for (int a = 0; a < 5; ++a)
            for (int b = 0; b < 5; ++b)
                for (int c = 0; c < 5; ++c)
                    for (int d = 0; d < 5; ++d) {
                        bool ok = false;
                        if (a + b == c + d) ok = true;
                        if (a + c == b + d) ok = true;
                        if (a + d == b + c) ok = true;
                        good[a][b][c][d] = ok;
                    }
        init = true;
    }

    /* DP arrays: index = a*25 + b*5 + c for triple (a,b,c) */
    seq_t dp[125], ndp[125];
    for (int i = 0; i < 125; ++i) dp[i] = (seq_t)1;   /* all triples are possible initially */

    for (idx_t len = 4; len <= L; ++len) {
        for (int i = 0; i < 125; ++i) ndp[i] = (seq_t)0;
        for (int a = 0; a < 5; ++a)
            for (int b = 0; b < 5; ++b)
                for (int c = 0; c < 5; ++c) {
                    const seq_t cur = dp[a * 25 + b * 5 + c];
                    if (cur == (seq_t)0) continue;
                    for (int d = 0; d < 5; ++d) {
                        if (!good[a][b][c][d]) continue;
                        ndp[b * 25 + c * 5 + d] += cur;
                    }
                }
        for (int i = 0; i < 125; ++i) dp[i] = ndp[i];
    }

    seq_t total = (seq_t)0;
    for (int i = 0; i < 125; ++i) total += dp[i];
    return total;
}
