/*
 * A352940  The largest positive integer k such that binomial(k+1,2) <=
 *          binomial(n,2)^2.
 * https://oeis.org/A352940
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (-1 + sqrt(1 + 8 * binomial(n,2)^2) ) / 2 )
 * Range:     u8 none, u16 n <= 7, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sqrt_floor(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    seq_t ans = 0;
    while (low <= high) {
        seq_t mid = low + (high - low) / 2;
        seq_t mid_sq = mid * mid;
        if (mid_sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* b = binomial(n,2) = n*(n-1)/2 */
    seq_t nn = (seq_t)n;
    seq_t b = nn * (nn - 1) / 2;

    /* D = 1 + 8*b^2 */
    seq_t d = 1 + 8 * b * b;

    seq_t s = sqrt_floor(d);

    /* k = floor((s - 1) / 2) */
    seq_t k = (s - 1) / 2;

    return k;
}
