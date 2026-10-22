/*
 * A314826  Coordination sequence Gal.6.129.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + 1 + C(n) where C(n) is the number of indices k (1 ≤
 *            k ≤ n) such that k ≡ 0, 1, 3, 9 (mod 11). (k = 1 is not counted
 *            because a(1) = 5.)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count how many times the step 5 occurs up to position n. */
    idx_t cnt;
    if (n <= 10) {
        /* Within the first 10 positions only indices 3 and 9 give step 5. */
        cnt = 0;
        if (n >= 3) cnt++;
        if (n >= 9) cnt++;
    } else {
        /* First ten indices contribute two steps of 5 (at 3 and 9). */
        cnt = 2;
        idx_t m = n - 10;          /* indices 11..n inclusive */
        idx_t full_cycles = m / 11;
        cnt += full_cycles * 4;     /* 4 steps of 5 per full 11-cycle */
        idx_t rem = m % 11;         /* remaining indices after full cycles */
        for (idx_t k = 1; k <= rem; ++k) {
            idx_t r = k % 11;
            if (r == 1 || r == 2 || r == 4 || r == 10) {
                cnt++;
            }
        }
    }

    /* a(n) = 4*n + 1 + cnt */
    seq_t base = (seq_t)n * 4;
    seq_t add  = (seq_t)1 + (seq_t)cnt;
    return base + add;
}
