/*
 * A263367  Number of (n+1)X(2+1) 0..2 arrays with each row and column
 *          divisible by 7, read as a base-3 number with top and left being
 *          the most significant digits.
 * https://oeis.org/A263367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by dynamic programming over the 4 row patterns
 *            {000, 021, 112, 210}. Each pattern is a 3‑digit base‑3 number
 *            divisible by 7. An (n+1)×3 array over {0,1,2} satisfies the
 *            row/column divisibility iff the sequence of patterns of length
 *            n+1 returns the column remainders to 0 modulo 7....
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int patterns[4][3] = {
    {0, 0, 0},   /* 000   */
    {0, 2, 1},   /* 021   */
    {1, 1, 2},   /* 112   */
    {2, 1, 0}    /* 210   */
};

seq_t a(idx_t n)
{
    /* state i = x*49 + y*7 + z  (x,y,z in 0..6)  */
    static seq_t dp[343];
    static seq_t ndp[343];
    static int trans[343][4];
    static int trans_init = 0;

    if (!trans_init) {
        for (int i = 0; i < 343; ++i) {
            int x = i / 49;
            int rem = i % 49;
            int y = rem / 7;
            int z = rem % 7;
            for (int p = 0; p < 4; ++p) {
                int nx = (3 * x + patterns[p][0]) % 7;
                int ny = (3 * y + patterns[p][1]) % 7;
                int nz = (3 * z + patterns[p][2]) % 7;
                trans[i][p] = nx * 49 + ny * 7 + nz;
            }
        }
        trans_init = 1;
    }

    /* initialise dp for step 0 */
    for (int i = 0; i < 343; ++i) dp[i] = 0;
    dp[0] = 1;              /* state (0,0,0) */

    seq_t *curr = dp;
    seq_t *next = ndp;
    idx_t steps = n + 1;     /* number of rows */

    for (idx_t step = 0; step < steps; ++step) {
        /* zero next array */
        for (int i = 0; i < 343; ++i) next[i] = 0;

        for (int i = 0; i < 343; ++i) {
            if (curr[i] == 0) continue;
            seq_t val = curr[i];
            next[trans[i][0]] += val;
            next[trans[i][1]] += val;
            next[trans[i][2]] += val;
            next[trans[i][3]] += val;
        }

        /* swap arrays */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    return curr[0];          /* state (0,0,0) after all rows */
}
