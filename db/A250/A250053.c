/*
 * A250053  Number of length n+6 0..2 arrays with no seven consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining five terms.
 * https://oeis.org/A250053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+6 ternary (0..2) arrays with no seven
 *            consecutive terms having the maximum of any two terms equal to
 *            the minimum of the remaining five terms. Computed via dynamic
 *            programming over the last 6 terms (state space size 3^6)
 * Range:     u8 none, u16 n <= 8, u32 n <= 28, u64 n <= 67, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* static initialization */
    static int init = 0;
    static int pow3[6];
    static uint8_t digits[729][6];
    static uint8_t allowed[729][3];
    static seq_t cur[729];
    static seq_t nxt[729];
    if (!init) {
        init = 1;
        pow3[0] = 1;
        for (int i = 1; i < 6; ++i) pow3[i] = pow3[i-1] * 3;   // pow3[i] = 3^i
        /* precompute digits of each state */
        for (int s = 0; s < 729; ++s) {
            int rem = s;
            for (int i = 5; i >= 0; --i) {
                digits[s][i] = rem % 3;
                rem /= 3;
            }
        }
        /* precompute allowed transitions */
        for (int s = 0; s < 729; ++s) {
            int w[7];
            for (int i = 0; i < 6; ++i) w[i] = digits[s][i];
            for (int d = 0; d < 3; ++d) {
                w[6] = d;
                int ok = 1;
                /* check all pairs */
                for (int i = 0; i < 7 && ok; ++i) {
                    for (int j = i + 1; j < 7 && ok; ++j) {
                        int m12 = w[i] > w[j] ? w[i] : w[j];
                        int m5 = 2;
                        for (int k = 0; k < 7; ++k) {
                            if (k == i || k == j) continue;
                            if (w[k] < m5) m5 = w[k];
                        }
                        if (m12 == m5) ok = 0;
                    }
                }
                allowed[s][d] = (uint8_t)ok;
            }
        }
    }
    /* initialize DP for sequences of length 6 (all allowed) */
    for (int s = 0; s < 729; ++s) cur[s] = 1;
    /* number of extensions to reach length n+6 is n */
    for (idx_t step = 0; step < n; ++step) {
        for (int s = 0; s < 729; ++s) nxt[s] = 0;
        for (int s = 0; s < 729; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            for (int d = 0; d < 3; ++d) {
                if (!allowed[s][d]) continue;
                int new_s = (s % 243) * 3 + d;   /* shift left, drop oldest */
                nxt[new_s] += val;
            }
        }
        for (int s = 0; s < 729; ++s) cur[s] = nxt[s];
    }
    /* sum all sequences of length n+6 */
    seq_t total = 0;
    for (int s = 0; s < 729; ++s) total += cur[s];
    return total;
}
