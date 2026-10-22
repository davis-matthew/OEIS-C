/*
 * A322940  a(n) = [x^n] (4*x^2 + x - 1)/(2*x^2 + 3*x - 1).
 * https://oeis.org/A322940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) for n >= 3 with initial values
 *            a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)2;
    seq_t a2 = (seq_t)4;

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = (seq_t)3 * a2 + (seq_t)2 * a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
