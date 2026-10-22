/*
 * A396421  a(n) = r(n,n), where r(1,k)=f(k)=ceiling(k*c), r(n,k)=f(r(n-1,k)),
 *          c=4/3.
 * https://oeis.org/A396421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r(n, n), with r(1, k) = f(k) = ceil(k * 4 / 3) and r(n,
 *            k) = f(r(n-1, k)). Equivalently a(n) = f^n(n)
 * Range:     u8 n <= 10, u16 n <= 27, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence is indexed from 1; 0 is unused */
        return 0;
    seq_t v = (seq_t)n;  /* start with k = n */

    /* iterate f exactly n times */
    for (idx_t i = 0; i < n; ++i)
    {
        /* f(v) = ceil(v * 4 / 3) = floor((4*v + 2) / 3) */
        /* compute using integer arithmetic without overflow: */
        seq_t q = v / 3;
        seq_t r = v % 3;
        if (r == 0)          /* v = 3q */
            v = q * 4;
        else if (r == 1)     /* v = 3q + 1 -> 4q + 2 */
            v = q * 4 + 2;
        else                 /* r == 2 -> 3q + 2 -> 4q + 3 */
            v = q * 4 + 3;
    }
    return v;
}
