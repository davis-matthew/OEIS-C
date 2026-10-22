/*
 * A387767  a(n) = Sum_{k=0..n} 2^k * binomial(2*k+1,2*n-2*k+1).
 * https://oeis.org/A387767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 2^k * binomial(2*k+1,2*n-2*k+1) Recurrence:
 *            a(n) = 4*a(n-1) + 8*a(n-3) - 4*a(n-4) (n ≥ 4) Initial values:
 *            a(0)=1, a(1)=6, a(2)=22, a(3)=96
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)22;
    if (n == 3) return (seq_t)96;

    seq_t a0 = (seq_t)1, a1 = (seq_t)6, a2 = (seq_t)22, a3 = (seq_t)96, next;
    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)4 * a3 + (seq_t)8 * a1 - (seq_t)4 * a0;
        a0 = a1; a1 = a2; a2 = a3; a3 = next;
    }
    return a3;
}
