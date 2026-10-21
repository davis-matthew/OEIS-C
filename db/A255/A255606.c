/*
 * A255606  Integer part of the area of a hexagon with side length n.
 * https://oeis.org/A255606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3*sqrt(3)/2)*n^2)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 31, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes floor(sqrt(x)) for any unsigned integer type seq_t. */
static seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = x;
    seq_t mid, ans = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        seq_t mid2 = mid * mid;
        if (mid2 <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* n must be >= 1 according to the sequence definition. */
    seq_t nn = (seq_t)n;
    seq_t t = nn * nn;          /* n^2 */
    t *= (seq_t)3;             /* 3*n^2 */
    seq_t val = t * t;           /* (3*n^2)^2 */
    val *= (seq_t)3;             /* 3*(3*n^2)^2 */
    seq_t sqrt_floor = floor_sqrt(val);  /* floor(sqrt(3*(3*n^2)^2)) */
    seq_t result = sqrt_floor / 2;        /* floor division by 2 */
    return result;
}
