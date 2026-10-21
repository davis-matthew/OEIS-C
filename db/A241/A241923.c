/*
 * A241923  6*s*t*(4*s^4 + 3*t^4), where s > 0, t = 1..s.
 * https://oeis.org/A241923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*s*t*(4*s^4 + 3*t^4), where s is the smallest integer
 *            with s*(s+1)/2 ≥ n, and t = n - s*(s-1)/2
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 252, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t s   = 1;
    /* find s such that s*(s+1)/2 >= n */
    while ((seq_t)s * (s + 1) / 2 < n)
        ++s;

    idx_t prev = (s - 1) * s / 2;
    idx_t t    = n - prev;   /* 1 <= t <= s */

    seq_t ss = s;
    seq_t tt = t;

    /* compute ss^4 and tt^4 */
    seq_t ss2 = ss * ss;
    seq_t ss4 = ss2 * ss2;
    seq_t tt2 = tt * tt;
    seq_t tt4 = tt2 * tt2;

    seq_t tmp = 4 * ss4 + 3 * tt4;
    seq_t val = 6 * ss * tt * tmp;

    return val;
}
