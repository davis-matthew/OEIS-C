/*
 * A247921  Number of length n+4 0..2 arrays with some disjoint pairs in every
 *          consecutive five terms having the same sum.
 * https://oeis.org/A247921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of length n+4 arrays over {0,1,2} such that
 *            in every consecutive block of five terms there exist two
 *            disjoint pairs with equal sum. The function computes this count
 *            by dynamic programming on the last four digits
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 18, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Helper: returns true iff the 5‑tuple v[0]..v[4] contains two
   disjoint pairs with the same sum. */
static bool window_ok(int v[5]) {
    /* Precompute the 10 pair sums and masks */
    int masks[10];
    int sums[10];
    int idx = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            masks[idx] = (1 << i) | (1 << j);
            sums[idx]  = v[i] + v[j];
            ++idx;
        }
    }
    for (int p = 0; p < 10; ++p) {
        for (int q = p + 1; q < 10; ++q) {
            if ((masks[p] & masks[q]) == 0 && sums[p] == sums[q]) {
                return true;
            }
        }
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) {          /* offset is 1; value undefined for n==0 */
        return (seq_t)0;
    }

    /* dp[s] = number of valid sequences ending with last 4 digits
       encoded by state s (0..80). */
    seq_t dp[81] = {0};
    seq_t ndp[81] = {0};

    /* --------------------------------------------------------------------
       Precompute transition legality: valid[s][x] is true iff
       extending the state s by digit x yields a 5‑tuple that satisfies
       the window property.  This is done once.  */
    static bool valid[81][3] = {{0}};
    static bool init = false;
    if (!init) {
        for (int s = 0; s < 81; ++s) {
            int a0 = (s / 27) % 3;
            int a1 = (s / 9) % 3;
            int a2 = (s / 3) % 3;
            int a3 = s % 3;
            for (int x = 0; x < 3; ++x) {
                int v[5] = { a0, a1, a2, a3, x };
                valid[s][x] = window_ok(v);
            }
        }
        init = true;
    }

    /* --------------------------------------------------------------------
       Build initial counts for all 5‑length sequences that satisfy
       the window property.  Each such sequence contributes to the
       state represented by its last four digits. */
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c)
                for (int d = 0; d < 3; ++d)
                    for (int e = 0; e < 3; ++e) {
                        int v[5] = { a, b, c, d, e };
                        if (!window_ok(v)) continue;
                        int ns = b * 27 + c * 9 + d * 3 + e;  /* last four digits */
                        dp[ns] += (seq_t)1;
                    }

    /* --------------------------------------------------------------------
       Extend the sequences by (n-1) further digits.  For n==1 the
       loop is skipped, leaving dp containing counts for length 5. */
    for (idx_t step = 0; step < n - 1; ++step) {
        for (int i = 0; i < 81; ++i) ndp[i] = (seq_t)0;
        for (int s = 0; s < 81; ++s) {
            seq_t cur = dp[s];
            if (!cur) continue;
            for (int x = 0; x < 3; ++x) {
                if (!valid[s][x]) continue;
                int ns = (s % 27) * 3 + x;            /* drop oldest digit */
                ndp[ns] += cur;
            }
        }
        for (int i = 0; i < 81; ++i) dp[i] = ndp[i];
    }

    /* --------------------------------------------------------------------
       Sum all states to obtain the total number of valid arrays. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 81; ++i) total += dp[i];
    return total;
}
