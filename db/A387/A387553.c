/*
 * A387553  a(n) = (1/2) * Sum_{k=0..floor(n/2)} 2^(n-k) *
 *          binomial(2*k+2,2*n-4*k+1).
 * https://oeis.org/A387553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) + 8*a(n-3) - 4*a(n-4) + 16*a(n-5) - 16*a(n-6),
 *            a(0)=1, a(1)=0, a(2)=4, a(3)=8, a(4)=12, a(5)=80
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)12;
    if (n == 5) return (seq_t)80;

    seq_t a0 = 1;
    seq_t a1 = 0;
    seq_t a2 = 4;
    seq_t a3 = 8;
    seq_t a4 = 12;
    seq_t a5 = 80;
    seq_t ai;

    for (idx_t i = 6; i <= n; ++i) {
        ai = 4*a4 + 8*a3 - 4*a2 + 16*a1 - 16*a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = ai;
    }

    return ai;
}
