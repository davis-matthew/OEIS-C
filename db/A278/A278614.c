/*
 * A278614  Sum of terms in level n of TRIP - Stern sequence associated with
 *          permutation triple (e,12,12).
 * https://oeis.org/A278614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=3, a(1)=8, a(2)=22; a(n)=3*a(n-1)+a(n-2)-4*a(n-3) for n>=3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)22;

    seq_t a0 = 3, a1 = 8, a2 = 22, ai;
    for (idx_t i = 3; i <= n; ++i) {
        ai = 3 * a2 + a1;
        ai -= 4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
