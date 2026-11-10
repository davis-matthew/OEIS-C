/*
 * A379406  a(n) = A379511(n) - A378142(n).
 * https://oeis.org/A379406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(2^(1/4)+2^(-1/4))) - floor(n*2^(1/4)) -
 *            floor(n*2^(-1/4)), i.e. 1 if {n*2^(1/4)} + {n*2^(-1/4)} >= 1,
 *            else 0. (Fitted to the listed terms: the stated A379511 -
 *            A378142 gives a(1) = -1, which contradicts a(1) = 1.)
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A379406_floor_nr(seq_t n)
{
    /* floor(n*2^(1/4)): largest p with p^4 <= 2 n^4 */
    seq_t n2 = n * n;
    seq_t target = 2 * n2 * n2;
    seq_t lo = 0, hi = 2 * n;
    while (lo < hi)
    {
        seq_t mid = (lo + hi + 1) / 2;
        seq_t m2 = mid * mid;
        if (m2 * m2 <= target)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

static seq_t A379406_floor_n_over_r(seq_t n)
{
    /* floor(n/2^(1/4)): largest q with 2 q^4 <= n^4 */
    seq_t n2 = n * n;
    seq_t target = n2 * n2;
    seq_t lo = 0, hi = n;
    while (lo < hi)
    {
        seq_t mid = (lo + hi + 1) / 2;
        seq_t m2 = mid * mid;
        if (2 * m2 * m2 <= target)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t p = A379406_floor_nr(nn);
    seq_t q = A379406_floor_n_over_r(nn);
    seq_t m = p + q + 1;
    /* a(n)=1 iff m <= n*(r+1/r)  <=>  m*r <= n*(1+sqrt2)
       <=> sqrt2*(m^2-2n^2) <= 3n^2 */
    seq_t m2 = m * m;
    seq_t t = 2 * nn * nn;
    if (m2 <= t)
        return 1;
    seq_t d = m2 - t;
    seq_t n4 = nn * nn * nn * nn;
    return (2 * d * d <= 9 * n4) ? 1 : 0;
}
