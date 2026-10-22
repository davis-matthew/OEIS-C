/*
 * A375995  Number of subwords of the form UUUU in nondecreasing Dyck paths of
 *          length 2n.
 * https://oeis.org/A375995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*(n-3)*L(2n-5)-3*F(2n-6))/5 for n>=3; a(0)=a(1)=a(2)=0
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;

    /* 2n-5 is ≥ 1 for n≥3 and fits in idx_t for all n considered. */
    idx_t m = 2 * n - 5;

    /* Compute F(m) and F(m+1) iteratively. */
    seq_t f_m = 0;    /* F(0)  */
    seq_t f_m1 = 1;   /* F(1)  */

    for (idx_t i = 2; i <= m + 1; ++i)
    {
        seq_t tmp = f_m + f_m1;
        f_m   = f_m1;
        f_m1  = tmp;
    }
    /* After the loop: f_m = F(m), f_m1 = F(m+1) */

    /* Lucas number: L(m) = 2*F(m+1) - F(m) */
    seq_t L_m = (seq_t)2 * f_m1 - f_m;

    /* F(m-1) = F(m+1) - F(m) */
    seq_t F_m_minus_1 = f_m1 - f_m;

    /* Compute numerator: 2*(n-3)*L(m) - 3*F(m-1) */
    seq_t part1 = (seq_t)2 * (seq_t)(n - 3) * L_m;
    seq_t part2 = (seq_t)3 * F_m_minus_1;
    seq_t numerator = part1 - part2;

    /* Division by 5 (exact) */
    return numerator / (seq_t)5;
}
