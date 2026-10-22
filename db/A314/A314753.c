/*
 * A314753  Coordination sequence Gal.4.52.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 4*n + Σ_{i=0}^{n-1} δ(i) with δ(i) = 0,0,0,2,0,0,1
 *            for i = 0…6 1,0,0,2,0,0,1 for i ≥ 7 in repeating cycles. The
 *            δ(i) repeat every 7 terms after the initial 7 entries
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t delta_pre[8]   = {0,0,0,2,0,0,1,0};   /* prefix sums; delta_pre[7] = 0 unused */
static const seq_t delta_pre_sum[8] = {0,0,0,0,2,2,2,3};

static const seq_t delta_cycle[8]   = {1,0,0,2,0,0,1,0};   /* cycle sums; delta_cycle[7] = 0 unused */
static const seq_t delta_cycle_sum[8] = {0,1,1,1,3,3,3,4};

seq_t a(idx_t n)
{
    /* a(0) = 1; for n > 0:
       a(n) = 1 + 4*n + sum_{i=0}^{n-1} δ(i)   */
    seq_t sum_delta;

    if (n < 7)
    {
        /* use the prefix part directly */
        sum_delta = delta_pre_sum[n];
    }
    else
    {
        idx_t rem  = (n - 7) % 7;          /* remaining elements after full cycles */
        idx_t cyc  = (n - 7) / 7;          /* number of full 7‑term cycles */

        /* total Δ sum = initial 7‑term sum + full cycles + partial cycle */
        sum_delta = delta_pre_sum[7]          /* = 3 */
                  + (seq_t)cyc * 4            /* each full cycle contributes 4 */
                  + delta_cycle_sum[rem];
    }

    return (seq_t)1 + (seq_t)4 * n + sum_delta;
}
