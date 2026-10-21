/*
 * A271895  Expansion of (1-2*x^2) / ( 1-2*x-4*x^2+6*x^3 ).
 * https://oeis.org/A271895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - 6*a(n-3) (n ≥ 3); a(0) = 1, a(1) =
 *            2, a(2) = 6
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;

    seq_t a0 = (seq_t)1;      /* a(n-3) */
    seq_t a1 = (seq_t)2;      /* a(n-2) */
    seq_t a2 = (seq_t)6;      /* a(n-1) */
    seq_t a3;                 /* a(n)   */

    for (idx_t i = 3; i <= n; ++i) {
        a3 = (seq_t)2 * a2 + (seq_t)4 * a1 - (seq_t)6 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
