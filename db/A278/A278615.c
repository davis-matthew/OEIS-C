/*
 * A278615  Sum of terms in level n of TRIP - Stern sequence associated with
 *          permutation triple (e,13,23).
 * https://oeis.org/A278615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+4*a(n-2)-6*a(n-3) for n>=3, with initial values
 *            a(0)=3, a(1)=8, a(2)=21
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)21;

    seq_t a0 = (seq_t)3, a1 = (seq_t)8, a2 = (seq_t)21;
    seq_t ai;
    idx_t i;
    for (i = 3; i <= n; ++i) {
        ai = (seq_t)2 * a2 + (seq_t)4 * a1 - (seq_t)6 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
