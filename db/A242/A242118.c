/*
 * A242118  Number of unit squares that intersect the circumference of a
 *          circle of radius n centered at (0,0).
 * https://oeis.org/A242118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * Sum_{k=1..n} ( ceil(sqrt(n^2 - (k-1)^2)) -
 *            floor(sqrt(n^2 - k^2)) )
 * Range:     u8 n <= 32, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes integer square root: floor(sqrt(v)) for 0 <= v < 2^64 */
static idx_t isqrt_u64(idx_t v)
{
    idx_t low = 0, high = v, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {          /* sqrt(0) == 0 */
            ans = 0;
            low = 1;
            continue;
        }
        /* avoid overflow: mid*mid <= v  <=>  v / mid >= mid */
        if (v / mid < mid) {
            high = mid - 1;
        } else {
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

/* OEIS sequence A242118: number of unit squares intersecting the circumference of a circle of radius n */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t nn = (idx_t)n * n;             /* n^2 */
    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t i = k - 1;
        idx_t sub1 = nn - (idx_t)i * i;  /* n^2 - (k-1)^2 */
        idx_t sub2 = nn - (idx_t)k * k;  /* n^2 - k^2 */

        idx_t f1 = isqrt_u64(sub1);     /* floor sqrt */
        if ((idx_t)f1 * f1 != sub1) ++f1;   /* ceil sqrt */

        idx_t f2 = isqrt_u64(sub2);     /* floor sqrt */

        sum += (seq_t)f1 - (seq_t)f2;
    }

    return sum * 4;
}
