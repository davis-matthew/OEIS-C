/*
 * A248188  Numbers k such that A248186(k+1) = A248186(k) + 1.
 * https://oeis.org/A248188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+2) * ³√3) (i.e. the Beatty sequence for √[3]3),
 *            except that a(656) = 948 (one less than the value given by the
 *            above formula)
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 699, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute integer floor of cube root.  */
static seq_t floor_cuberoot(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* Find an upper bound.  */
    while (hi * hi * hi <= x)
        hi <<= 1;

    /* Binary search for the largest value with mid³ ≤ x.  */
    while (lo < hi) {
        seq_t mid = lo + ((hi - lo + 1) >> 1);
        seq_t mid3 = mid * mid * mid;
        if (mid3 <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Small special case that occurs in the OEIS data.  */
    if (n == 656)
        return (seq_t)948;

    /* m = n+2.  Compute floor((n+2)*³√3).  */
    idx_t m = n + 2;
    seq_t big = (seq_t)3 * (seq_t)m * (seq_t)m * (seq_t)m;
    return floor_cuberoot(big);
}
