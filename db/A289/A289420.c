/*
 * A289420  Number of Dyck paths of semilength n and height exactly 7.
 * https://oeis.org/A289420
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 20, u64 n <= 37, u128 n <= 72,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 880 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t compute_leq(idx_t n, int H)
{
    /* H is at most 7; arrays sized for the maximum. */
    seq_t dp[8];
    seq_t nxt[8];
    for (int i = 0; i <= H; ++i) dp[i] = 0;
    dp[0] = 1;                     /* start at height 0, zero steps taken */

    idx_t steps = n * 2;            /* total up/down steps in a Dyck path */
    for (idx_t step = 0; step < steps; ++step)
    {
        for (int i = 0; i <= H; ++i) nxt[i] = 0;
        for (int h = 0; h <= H; ++h)
        {
            seq_t val = dp[h];
            if (val == 0) continue; /* skip empty state – works for any seq_t */
            if (h < H) nxt[h + 1] += val;   /* up step */
            if (h > 0) nxt[h - 1] += val;   /* down step */
        }
        for (int i = 0; i <= H; ++i) dp[i] = nxt[i];
    }
    return dp[0];                 /* back to ground level at the end */
}
/* Implementation: a(n) = number of Dyck paths of semilength n and
   height exactly 7.
   For each n ≥ 7 we count Dyck paths whose maximum height is ≤ 7 and
   ≤ 6 with a standard dynamic‑programming walk on up/down steps.
   The desired count is f(n,7) – f(n,6).  Initial values:
   a(7)=1, a(8)=13, a(9)=102, a(10)=629, a(11)=3366, a(12)=16422, a(13)=75140. */

seq_t a(idx_t n)
{
    if (n < 7) return 0;          /* behavior below the offset is irrelevant */
    seq_t le7 = compute_leq(n, 7); /* paths with max height ≤ 7   */
    seq_t le6 = compute_leq(n, 6); /* paths with max height ≤ 6   */
    return le7 - le6;             /* exactly height 7            */
}
