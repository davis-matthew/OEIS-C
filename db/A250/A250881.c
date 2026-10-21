/*
 * A250881  Number of (4+1) X (n+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4), initial terms:
 *            a(1)=8736, a(2)=30360, a(3)=73122, a(4)=144248. This routine
 *            computes a(n) by iterating the constant third difference,
 *            avoiding any signed arithmetic
 * Range:     u8 none, u16 n <= 2, u32 n <= 151, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 1) return (seq_t)8736;
    if (n == 2) return (seq_t)30360;
    if (n == 3) return (seq_t)73122;
    if (n == 4) return (seq_t)144248;

    /* Third difference is constant (7226).  These are the last known
       first and second differences at index 4. */
    seq_t val = (seq_t)144248;   /* a(4) */
    seq_t d1  = (seq_t)71126;    /* Δ1(4) = a(4)-a(3) */
    seq_t d2  = (seq_t)28364;    /* Δ2(4) = Δ1(4)-Δ1(3) */
    const seq_t d3 = (seq_t)7226;/* Δ3 constant for all n >= 4 */

    idx_t i;
    for (i = 5; i <= n; ++i) {
        d2 += d3;       /* Δ2(n) = Δ2(n-1) + d3 */
        d1 += d2;       /* Δ1(n) = Δ1(n-1) + Δ2(n) */
        val += d1;      /* a(n)   = a(n-1) + Δ1(n) */
    }
    return val;
}
