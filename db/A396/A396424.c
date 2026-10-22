/*
 * A396424  a(n) = r(n,n), where r(1,k)=f(k)=ceiling(k*c), r(n,k)=f(r(n-1,k)),
 *          c=5/3.
 * https://oeis.org/A396424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = fⁿ(n), where f(k) = ceil(k*5/3) = (5*k + 2)/3
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = (seq_t)n;
    for (idx_t i = 0; i < n; ++i)
    {
        v = (5 * v + 2) / 3;   /* f(v) = ceil(v*5/3) */
    }
    return v;
}
