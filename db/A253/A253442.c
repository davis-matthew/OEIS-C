/*
 * A253442  Expansion of x * (96 - 816*x) / ((1 - x) * (1 - 1442*x + x^2)) in
 *          powers of x.
 * https://oeis.org/A253442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1443*a(n-1) - 1443*a(n-2) + a(n-3) for n>3, with a(1)=96,
 *            a(2)=137712, a(3)=198579888
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)96;
    if (n == 2) return (seq_t)137712;
    if (n == 3) return (seq_t)198579888;

    seq_t a1 = (seq_t)96;
    seq_t a2 = (seq_t)137712;
    seq_t a3 = (seq_t)198579888;
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)((seq_t)1443 * a3 - (seq_t)1443 * a2 + a1);
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
