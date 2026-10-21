/*
 * A258638  Number of length 7 1..(n+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5 or 7.
 * https://oeis.org/A258638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑7 arrays with entries in 1..(n+1) such
 *            that every leading partial sum is divisible by 2, 3, 5, or 7.
 *            This function evaluates the sum for any n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 30, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Number of available step values */
    idx_t N = n + 1;

    /* Pre‑compute how many step values give each residue d = a mod 210.
       cnt[d] = |{ a ∈ [1,N] : a ≡ d (mod 210) }|   (0 ≤ d < 210) */
    uint16_t cnt[210];
    for (int d = 0; d < 210; ++d) {
        if (d == 0) {
            cnt[d] = (uint16_t)(N / 210);
        } else if ((idx_t)d <= N) {
            cnt[d] = (uint16_t)(1 + (N - d) / 210);
        } else {
            cnt[d] = 0;
        }
    }

    /* Enumerate the residues that are allowed as partial sums
       (divisible by 2, 3, 5, or 7).  There are 162 of them. */
    int residues[210];
    int id[210];
    int rc = 0;
    for (int r = 0; r < 210; ++r) id[r] = -1;
    for (int r = 0; r < 210; ++r) {
        if (r % 2 == 0 || r % 3 == 0 || r % 5 == 0 || r % 7 == 0) {
            residues[rc] = r;
            id[r] = rc;
            ++rc;
        }
    }
    int R = rc;                     /* 162 */

    /* DP vectors: vec[r] = number of ways to reach residue r after the current number
       of steps (initially 0 steps).  We use arrays of size 210 to avoid dynamic allocation. */
    seq_t vec[210] = {0};
    seq_t nvec[210];
    vec[id[0]] = 1;                 /* start with sum 0 (which is allowed) */

    /* Perform 7 steps.  Each step multiplies by the transition matrix
       whose entries depend only on the residue difference and on cnt. */
    for (int step = 0; step < 7; ++step) {
        for (int i = 0; i < R; ++i) nvec[i] = 0;

        for (int i = 0; i < R; ++i) {
            seq_t v = vec[i];
            if (v == 0) continue;
            int r = residues[i];

            for (int j = 0; j < R; ++j) {
                int r2 = residues[j];
                int d = r2 - r;
                if (d < 0) d += 210;          /* difference modulo 210 */
                uint16_t c = cnt[d];
                if (c == 0) continue;
                nvec[j] += v * (seq_t)c;
            }
        }

        for (int i = 0; i < R; ++i) vec[i] = nvec[i];
    }

    /* Sum all ending residues to get the total number of sequences. */
    seq_t result = 0;
    for (int i = 0; i < R; ++i) result += vec[i];
    return result;
}
