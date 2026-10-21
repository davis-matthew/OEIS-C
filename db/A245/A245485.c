/*
 * A245485  a(n) = 1 if n is a square, -1 if n is seven times a square, 0
 *          otherwise.
 * https://oeis.org/A245485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = 1 if n is a square, -1 if n is 7 times a square, 0
 *            otherwise
 * Range:     i8 n <= 9063, i16 n <= 9063, i32 n <= 9063, i64 n <= 9063,
 *            i128 n <= 9063, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t lo = 1, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid > n / mid) {
            hi = mid - 1;
        } else {
            ans = mid;
            lo = mid + 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* offset starts at 1, but guard for completeness */

    /* check if n is a perfect square */
    idx_t sq = isqrt_u64(n);
    if (sq * sq == n)
        return 1;

    /* check if n is seven times a perfect square */
    if (n % 7 == 0) {
        idx_t m = n / 7;
        idx_t sm = isqrt_u64(m);
        if (sm * sm == m)
            return -1;
    }

    return 0;
}
