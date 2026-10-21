/*
 * A258636  Number of length 5 1..(n+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5 or 7.
 * https://oeis.org/A258636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑5 arrays over [1..n+1] whose * every
 *            leading partial sum is divisible by 2, 3, 5 or 7. * The function
 *            counts all such arrays by dynamic programming * over residues
 *            modulo lcm(2,3,5,7)=210
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 108, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* residues modulo 210 that are allowed */
    static bool allowed[210];
    static bool initialized = false;
    if (!initialized) {
        for (int r = 0; r < 210; ++r)
            allowed[r] = (r % 2 == 0) || (r % 3 == 0) ||
                         (r % 5 == 0) || (r % 7 == 0);
        initialized = true;
    }

    /* Count how many numbers in [1..n+1] are ≡ d (mod 210) */
    idx_t N = n + 1;                /* upper bound on each element (inclusive) */
    seq_t diffCnt[210];
    for (int d = 0; d < 210; ++d) {
        if (d == 0) {
            /* numbers ≡ 0 (mod 210) start at 210 */
            diffCnt[0] = (seq_t)(N / 210);
        } else if (N < (idx_t)d) {
            diffCnt[d] = 0;
        } else {
            diffCnt[d] = 1 + (seq_t)((N - (idx_t)d) / 210);
        }
    }

    /* DP over residues of the current prefix sum */
    seq_t cur[210] = {0}, nxt[210] = {0};
    cur[0] = 1;                      /* start from sum 0 */

    for (int step = 0; step < 5; ++step) {
        for (int r = 0; r < 210; ++r) nxt[r] = 0;
        for (int r = 0; r < 210; ++r) {
            seq_t ways = cur[r];
            if (!ways) continue;
            for (int d = 0; d < 210; ++d) {
                seq_t addcnt = diffCnt[d];
                if (!addcnt) continue;
                int rnew = r + d;
                if (rnew >= 210) rnew -= 210;
                if (!allowed[rnew]) continue;
                nxt[rnew] += ways * addcnt;
            }
        }
        for (int r = 0; r < 210; ++r) cur[r] = nxt[r];
    }

    /* Sum all residues that satisfy the divisibility condition */
    seq_t total = 0;
    for (int r = 0; r < 210; ++r) total += cur[r];
    return total;
}
