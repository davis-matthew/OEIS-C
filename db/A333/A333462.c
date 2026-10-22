/*
 * A333462  a(n) is the number of Gaussian integers z such that (n-1)/2 < |z|
 *          <= n/2, divided by 4.
 * https://oeis.org/A333462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( N(n/2) – N((n-1)/2) ) / 4 where N(r) is the number of
 *            Gaussian integers z with |z| ≤ r
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 1876, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* integer square root for arbitrary-precision seq_t */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    seq_t ans = 0;
    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);
        seq_t mid_sq = mid * mid;
        if (mid_sq == x) {
            return mid;
        }
        if (mid_sq < x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* Count lattice points with |z| ≤ n/2  (n is 2*r). */
static seq_t count_radius(idx_t n)
{
    seq_t V = (seq_t)n * (seq_t)n;          /* V = (2r)^2 = n^2            */
    idx_t maxx = n / 2;                    /* floor(r)                     */
    seq_t total = 0;

    for (idx_t x = 0; x <= maxx; ++x) {
        seq_t x2   = (seq_t)x * (seq_t)x;
        seq_t w    = V - (seq_t)4 * x2;      /* w = V - 4*x^2                */
        seq_t sq_w = isqrt_seq(w);          /* floor(sqrt(w))               */
        seq_t ymax = sq_w >> 1;             /* floor(sqrt(w)/2) = floor(ymax) */

        seq_t cnt = (x == 0) ? (2 * ymax + 1)
                            : (2 * (2 * ymax + 1));
        total += cnt;
    }
    return total;
}

seq_t a(idx_t n)
{
    seq_t N2 = count_radius(n);       /* |z| ≤ n/2                 */
    seq_t N1 = count_radius(n - 1);   /* |z| ≤ (n-1)/2             */
    return (N2 - N1) >> 2;            /* divide by 4                */
}
