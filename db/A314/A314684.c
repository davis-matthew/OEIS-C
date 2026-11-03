/*
 * A314684  Coordination sequence Gal.6.149.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 4*n + c(n) c(n) is the number of indices k (1 ≤ k ≤
 *            n) where the increment is 5. These indices form a periodic
 *            pattern with offset 5: 5,7,11,12,16,18,22,23,27,29,33,34,...
 *            (period 11, 4 indices per block) The count c(n) can be computed
 *            in O(1) using integer arithmetic
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t count_5(idx_t n)
{
    if (n < 5) return 0;
    idx_t m = n - 5;          /* offset from first `5` */
    idx_t q = m / 11;         /* number of complete 11‑step blocks */
    idx_t r = m % 11;         /* remaining offset within a block   */
    /* Base offsets within a block: 0,2,6,7 (add 5 to get actual indices) */
    idx_t limit = 5 + r;      /* maximum index reachable in current block */
    idx_t extra = 0;
    if (limit >= 5)  extra++; /* 5   */
    if (limit >= 7)  extra++; /* 7   */
    if (limit >= 11) extra++; /* 11  */
    if (limit >= 12) extra++; /* 12  */
    return q * 4 + extra;     /* 4 per full block, plus remaining ones   */
}

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 4;            /* 4*n part */
    seq_t cnt  = (seq_t)count_5(n);      /* extra 5‑increments count */
    return 1 + base + cnt;               /* a(0)=1, a(n)=1+4*n+cnt for n≥0 */
}
