/*
 * A384646  Expansion of (1+x) / (1-x-5*x^2-2*x^3).
 * https://oeis.org/A384646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = a(n-1) + 5*a(n-2) + 2*a(n-3) (n ≥ 3) with a(0) = 1, a(1)
 *            = 2, a(2) = 7
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)2;
    if (n == 2u) return (seq_t)7;

    seq_t a0 = (seq_t)1;   /* a(n-3)   */
    seq_t a1 = (seq_t)2;   /* a(n-2)   */
    seq_t a2 = (seq_t)7;   /* a(n-1)   */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = a2 + (seq_t)5 * a1 + (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
