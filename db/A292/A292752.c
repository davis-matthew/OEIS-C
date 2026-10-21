/*
 * A292752  Dimensions of centralizer algebras of groups associated with
 *          Z_4-codes.
 * https://oeis.org/A292752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (57+6*5^n+9^n)/96 (n>0) with a(0)=1. Recurrence: a(n) =
 *            15*a(n-1) - 59*a(n-2) + 45*a(n-3) for n>3, initial values:
 *            a(0)=1, a(1)=1, a(2)=3, a(3)=16
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)16;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)3;
    seq_t a3 = (seq_t)16;

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t cur = (seq_t)15 * a3;
        cur -= (seq_t)59 * a2;
        cur += (seq_t)45 * a1;
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }
    return a3;
}
