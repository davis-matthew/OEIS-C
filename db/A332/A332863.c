/*
 * A332863  Total binary weight squared of all A005251(n) binary sequences of
 *          length n not containing any isolated 1's.
 * https://oeis.org/A332863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n} k^2 * A097230(n,k)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 29, u64 n <= 66, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A332863: total binary weight squared of all binary strings of length n
   with no isolated 1's.  The DP keeps for each of the three states
   (S0 – last bit 0 or start, S1 – a single 1, S2 – a run of ≥2 1's)
   the count of strings, the sum of their weights, and the sum of the
   squares of their weights.  Transitions are updated in constant time
   per position, yielding an O(n) algorithm. */
#include <stddef.h>

static void upd(seq_t *tcnt, seq_t *tsumw, seq_t *tsumw2,
                seq_t src_cnt, seq_t src_sumw, seq_t src_sumw2,
                seq_t b)
{
    *tcnt   += src_cnt;
    *tsumw  += src_sumw + src_cnt * b;
    *tsumw2 += src_sumw2 + (seq_t)2 * b * src_sumw + src_cnt * b * b;
}

extern seq_t a(idx_t n)
{
    const int S0 = 0, S1 = 1, S2 = 2;
    seq_t c[3]   = {0, 0, 0};
    seq_t sw[3]  = {0, 0, 0};
    seq_t sw2[3] = {0, 0, 0};

    /* start with the empty string in state S0 */
    c[S0] = (seq_t)1;

    for (idx_t pos = 0; pos < n; ++pos)
    {
        seq_t nc[3]   = {0, 0, 0};
        seq_t nsw[3]  = {0, 0, 0};
        seq_t nsw2[3] = {0, 0, 0};

        /* From S0: add 0 → S0, add 1 → S1 */
        upd(&nc[S0], &nsw[S0], &nsw2[S0], c[S0], sw[S0], sw2[S0], (seq_t)0);
        upd(&nc[S1], &nsw[S1], &nsw2[S1], c[S0], sw[S0], sw2[S0], (seq_t)1);

        /* From S1: add 1 → S2  (adding 0 would create an isolated 1) */
        upd(&nc[S2], &nsw[S2], &nsw2[S2], c[S1], sw[S1], sw2[S1], (seq_t)1);

        /* From S2: add 0 → S0, add 1 → S2 */
        upd(&nc[S0], &nsw[S0], &nsw2[S0], c[S2], sw[S2], sw2[S2], (seq_t)0);
        upd(&nc[S2], &nsw[S2], &nsw2[S2], c[S2], sw[S2], sw2[S2], (seq_t)1);

        /* copy results back for next iteration */
        for (int i = 0; i < 3; ++i) {
            c[i]   = nc[i];
            sw[i]  = nsw[i];
            sw2[i] = nsw2[i];
        }
    }

    /* accepted strings end in S0 or S2 */
    return sw2[S0] + sw2[S2];
}
