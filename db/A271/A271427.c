/*
 * A271427  a(n) = 7^n - a(n-1) for n>0, a(0)=0.
 * https://oeis.org/A271427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - a(n-1) (a(0)=0) Equivalent linear recurrence: a(n)
 *            = 6*a(n-1) + 7*a(n-2), a(0)=0, a(1)=7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 7;

    seq_t a0 = 0;      // a(n-2)
    seq_t a1 = 7;      // a(n-1)
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = 6 * a1 + 7 * a0;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
