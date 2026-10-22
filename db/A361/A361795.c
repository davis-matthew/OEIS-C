/*
 * A361795  a(n) is the area of the largest rectangle with integer sides that
 *          can be drawn inside a circle of diameter n.
 * https://oeis.org/A361795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n/√2⌋² or ⌊n/√2⌋·(⌊n/√2⌋+1) depending on whether
 *            ⌊n/√2⌋²+ (⌊n/√2⌋+1)² ≤ n²
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root of a seq_t: 0 ≤ x ≤ max(seq_t). */
static inline seq_t _isqrt_seq(seq_t x)
{
    seq_t low = 0, high = x, mid, ans = 0;
    while (low <= high)
    {
        mid = low + (high - low) / (seq_t)2;
        seq_t mid_sq = mid * mid;
        if (mid_sq <= x)
        {
            ans = mid;
            low = mid + (seq_t)1;
        }
        else
        {
            if (mid == (seq_t)0) break;
            high = mid - (seq_t)1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;           /* convert index to seq_t */
    seq_t sq = sn * sn;            /* n²                                 */
    seq_t half = sq / (seq_t)2;    /* floor(n² / 2)                      */
    seq_t x = _isqrt_seq(half);    /* floor(n/√2)                        */

    seq_t x_sq = x * x;            /* x²                                */
    seq_t x1 = x + (seq_t)1;
    seq_t x1_sq = x1 * x1;         /* (x+1)²                            */

    /* Check whether x² + (x+1)² ≤ n² to decide which product gives
       the maximal area.                                           */
    if (x_sq + x1_sq <= sq)
        return x * x1;             /* x*(x+1)                          */
    else
        return x_sq;               /* x²                                */
}
