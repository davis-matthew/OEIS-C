/*
 * A280113  Triangular numbers (A000217) that are also centered 10-gonal
 *          numbers (A062786).
 * https://oeis.org/A280113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1443*a(n-1) - 1443*a(n-2) + a(n-3); Initial values:
 *            a(1)=1, a(2)=1711, a(3)=2467531
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1711;
    if (n == 3) return (seq_t)2467531;

    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)1711;
    seq_t a3 = (seq_t)2467531;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)1443 * a3 - (seq_t)1443 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
