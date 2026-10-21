/*
 * A250367  Number of length n+6 0..2 arrays with every seven consecutive
 *          terms having the maximum of some three terms equal to the minimum
 *          of the remaining four terms.
 * https://oeis.org/A250367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This computes a(n) by dynamic programming on the last six digits
 *            * of the array. The state is a base‑3 number of length 6; the
 *            transition * checks the window of length seven formed by the
 *            last six digits and the * new digit. The DP counts all sequences
 *            of length n+6 that satisfy the * condition
 * Range:     u8 none, u16 n <= 5, u32 n <= 18, u64 n <= 43, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Pre‑computed tables: win[g] is true if the 7‑digit window represented
 * by the base‑3 number g satisfies the condition.  shift[m] removes the
 * most significant digit from a 6‑digit base‑3 number. */
static bool win[2187];
static int  shift[729];

/* Build the tables once; it is safe to call from any thread due to
 * static storage and the local guard flag. */
static void init_tables(void)
{
    /* Compute 3^i for i up to 7 */
    int pw[8];
    pw[0] = 1;
    for (int i = 1; i < 8; ++i) pw[i] = pw[i-1] * 3;

    /* Pre‑compute shift: removing the highest digit of a 6‑digit number. */
    for (int m = 0; m < 729; ++m) shift[m] = m % 243;   /* 3^5 = 243 */

    /* Pre‑compute win for all 7‑digit windows. */
    for (int g = 0; g < 2187; ++g) {
        int digits[7];
        int t = g;
        for (int i = 0; i < 7; ++i) {            /* least significant first */
            digits[6-i] = t % 3;
            t /= 3;
        }
        bool ok = false;
        /* try all triples */
        for (int i = 0; i < 7 && !ok; ++i)
            for (int j = i+1; j < 7 && !ok; ++j)
                for (int k = j+1; k < 7 && !ok; ++k) {
                    int mx = digits[i];
                    if (digits[j] > mx) mx = digits[j];
                    if (digits[k] > mx) mx = digits[k];
                    int mn = 3;  /* larger than any digit */
                    for (int t = 0; t < 7; ++t)
                        if (t != i && t != j && t != k && digits[t] < mn)
                            mn = digits[t];
                    if (mx == mn) ok = true;
                }
        win[g] = ok;
    }
}

/* Main sequence function.  The function is re‑entrant; the tables are
 * built on the first call only. */
seq_t a(idx_t n)
{
    static bool tables_done = false;
    if (!tables_done) {
        init_tables();
        tables_done = true;
    }

    /* The total length of the arrays to count is n + 6 */
    const idx_t len = n + 6;

    /* DP states: dp[mask] is the number of ways to reach the state
     * represented by the 6‑digit base‑3 number mask. */
    seq_t dp[729];
    seq_t ndp[729];

    /* Initially, all 6‑digit masks are possible (length 6). */
    for (int i = 0; i < 729; ++i) dp[i] = 1;

    for (idx_t cur = 6; cur < len; ++cur) {
        /* zero ndp (safely for non‑trivial seq_t types). */
        for (int i = 0; i < 729; ++i) ndp[i] = 0;

        for (int mask = 0; mask < 729; ++mask) {
            seq_t cnt = dp[mask];
            if (cnt == 0) continue;
            for (int d = 0; d < 3; ++d) {
                /* window of 7 digits: append d to the end of mask */
                int win_index = mask * 3 + d;
                if (!win[win_index]) continue;
                int newmask = shift[mask] * 3 + d;
                ndp[newmask] += cnt;
            }
        }

        /* copy ndp to dp for next iteration */
        for (int i = 0; i < 729; ++i) dp[i] = ndp[i];
    }

    /* Sum all final states */
    seq_t total = 0;
    for (int i = 0; i < 729; ++i) total += dp[i];
    return total;
}
