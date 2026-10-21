/*
 * A258634  Number of length 3 1..(n+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5 or 7.
 * https://oeis.org/A258634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of length‑3 arrays of integers from 1..(n+1)
 *            whose every leading partial sum is divisible by 2, 3, 5 or 7.
 *            The algorithm counts all walks of length 3 on the residue graph
 *            modulo 210 (lcm(2,3,5,7)), using the fact that the residue of a
 *            partial sum determines all divisibility con...
 * Range:     u8 n <= 6, u16 n <= 51, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Needed for bool */
#include <stdbool.h>

/* Compute the sequence value for index n (n >= 1).  */
seq_t a(idx_t n)
{
    /* Pre‑compute the list of residues 0..209 that are divisible by
       2, 3, 5 or 7.  This is a static table that stays constant for all
       calls.  */
    static int  good_idx[162];
    static int  good_cnt = 0;
    if (!good_cnt) {
        int cnt = 0;
        for (int r = 0; r < 210; ++r) {
            if (r == 0 || r % 2 == 0 || r % 3 == 0 || r % 5 == 0 || r % 7 == 0)
                good_idx[cnt++] = r;
        }
        good_cnt = cnt;                /* should be 162 */
    }

    /* N = n+1 is the maximum element value.  */
    idx_t N      = n + 1;
    idx_t q      = N / 210;          /* number of full 210‑blocks   */
    idx_t rmod   = N % 210;          /* remainder in the last block */

    /* Count of values in 1..N that are congruent to each residue d (0..209).*/
    idx_t cnt_shift[210];
    for (int d = 0; d < 210; ++d) {
        /* Number of integers x (1 <= x <= N) with x % 210 == d.  */
        if (d == 0)
            cnt_shift[d] = q;
        else
            cnt_shift[d] = q + ((d <= rmod) ? 1 : 0);
    }

    /* U1[i] = number of ways to reach residue good_idx[i] after the first step. */
    seq_t U1[162] = {0};
    for (int i = 0; i < good_cnt; ++i) {
        int r = good_idx[i];
        U1[i] = (seq_t)cnt_shift[r];
    }

    /* U2[j] = number of ways to reach residue good_idx[j] after two steps. */
    seq_t U2[162] = {0};
    for (int i = 0; i < good_cnt; ++i) {
        seq_t u1 = U1[i];
        if (!u1) continue;
        int ri = good_idx[i];
        for (int j = 0; j < good_cnt; ++j) {
            int rj = good_idx[j];
            int delta = rj - ri;
            if (delta < 0) delta += 210;
            seq_t cnt = (seq_t)cnt_shift[delta];
            U2[j] += u1 * cnt;
        }
    }

    /* wsum[i] = total number of choices for the third element that keep
       the partial sum good, starting from residue good_idx[i]. */
    seq_t wsum[162] = {0};
    for (int i = 0; i < good_cnt; ++i) {
        int ri = good_idx[i];
        seq_t sum = 0;
        for (int k = 0; k < good_cnt; ++k) {
            int rk = good_idx[k];
            int delta = rk - ri;
            if (delta < 0) delta += 210;
            sum += (seq_t)cnt_shift[delta];
        }
        wsum[i] = sum;
    }

    /* Final result: sum over all reachable states after two steps,
       multiplied by the number of valid third elements. */
    seq_t result = 0;
    for (int i = 0; i < good_cnt; ++i)
        result += U2[i] * wsum[i];

    return result;
}
