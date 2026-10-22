/*
 * A362961  a(n) = Sum_{b=0..floor(sqrt(n)), n-b^2 is square} b.
 * https://oeis.org/A362961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{b=0..floor(sqrt(n)), n-b^2 is square} b
 * Range:     u8 n <= 2464, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t low = 1;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t div = n / mid;
        if (mid > div) {
            high = mid - 1;
        } else {
            if (mid + 1 > n / (mid + 1))
                return mid;
            low = mid + 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    idx_t maxb = isqrt_u64(n);
    seq_t sum = 0;
    for (idx_t b = 0; b <= maxb; ++b) {
        idx_t bb = b * b;
        idx_t rem = n - bb;
        idx_t s = isqrt_u64(rem);
        if (s * s == rem) {
            sum += (seq_t)b;
        }
    }
    return sum;
}
