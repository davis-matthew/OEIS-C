/*
 * A255849  Characteristic function of pentagonal numbers.
 * https://oeis.org/A255849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = m*(3*m-1)/2 for some m (m ≥ 0), otherwise 0;
 *            a(0) = 1
 * Range:     u8 n <= 9935, u16 n <= 9935, u32 n <= 9935, u64 n <= 9935,
 *            u128 n <= 9935, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt_u64(idx_t x)
{
    /* Binary search for floor(sqrt(x)).  x < 2^64, so sqrt(x) < 2^32. */
    idx_t low = 0;
    idx_t high = ((idx_t)1 << 32);      /* exclusive upper bound */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (mid == 0) {
            high = mid - 1;
            continue;
        }
        if (x / mid >= mid)   /* mid*mid <= x, without overflow */
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute floor(2*n/3) without overflow */
    idx_t t = (n / 3) * 2 + ((n % 3) == 2 ? 1 : 0);

    idx_t sqrt_t = isqrt_u64(t);   /* floor(sqrt(2*n/3)) */
    idx_t m = sqrt_t + 1;          /* m = floor(sqrt(2*n/3)) + 1 */

    seq_t mm = (seq_t)m;
    seq_t pent = mm * ((seq_t)3 * mm - 1) / (seq_t)2;

    return (pent == (seq_t)n) ? (seq_t)1 : (seq_t)0;
}
