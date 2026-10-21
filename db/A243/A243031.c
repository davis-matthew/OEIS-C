/*
 * A243031  Number of length n+3 0..6 arrays with no four elements in a row
 *          with pattern abba (possibly a=b) and new values 0..6 introduced in
 *          0..6 order.
 * https://oeis.org/A243031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A243031 – number of length n+3 arrays over {0,…,6} in which *
 *            new symbols appear in order 0,1,2,… (restricted growth string)
 *            and * no four consecutive elements form the pattern abba (a and
 *            b may be equal). * The sequence is computed by a
 *            dynamic‑programming approach over * states (a,b,c,max) wher...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define STATES 2401         /* 7⁴ states: (a,b,c,max) */

seq_t a(idx_t n)
{
    /* Valid for n ≥ 1; for n = 0 return 0 (undefined in OEIS) */
    if (n == 0) return 0;

    /* Target length of the arrays */
    const idx_t target_len = n + 3;

    /* Two DP rows: current and next */
    seq_t dp0[STATES] = {0};
    seq_t dp1[STATES] = {0};
    seq_t *cur  = dp0;
    seq_t *nxt  = dp1;

    /* Initialise all length‑3 sequences that satisfy the RGS rule */
    for (int a = 0; a < 7; ++a)
        for (int b = 0; b < 7; ++b)
            for (int c = 0; c < 7; ++c) {
                if (a != 0) continue;            /* start with 0 */

                int max1 = a;                     /* 0 */
                if (b > max1 + 1) continue;       /* b must be ≤ max+1 */
                int max2 = (b > max1) ? b : max1;
                if (c > max2 + 1) continue;       /* c must be ≤ max+1 */
                int max3 = (c > max2) ? c : max2;

                int idx = ((a * 7 + b) * 7 + c) * 7 + max3;
                cur[idx] = 1;
            }

    /* Extend the sequences step by step until length reaches target_len */
    for (idx_t step = 0; step < target_len - 3; ++step) {

        /* Clear the next row */
        for (int i = 0; i < STATES; ++i) nxt[i] = 0;

        /* Transition from every state in the current row */
        for (int i = 0; i < STATES; ++i) {
            seq_t val = cur[i];
            if (val == 0) continue;

            /* Decode (a,b,c,max) from the state index i */
            int rem = i;
            int a   = rem / (7*7*7);
            rem    %= (7*7*7);
            int b   = rem / (7*7);
            rem    %= (7*7);
            int c   = rem / 7;
            int max = rem % 7;

            int dmax = (max < 6) ? max + 1 : 6;   /* next symbol can be ≤ 6 and at most one larger than current max */

            for (int d = 0; d <= dmax; ++d) {
                if (d == a && b == c) continue;     /* forbid abba */

                int new_max = max;
                if (d == max + 1 && d <= 6) new_max = max + 1;

                int nidx = ((b * 7 + c) * 7 + d) * 7 + new_max;
                nxt[nidx] += val;
            }
        }

        /* Swap current and next rows */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum all states in the final row to obtain the answer */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i) total += cur[i];

    return total;
}
