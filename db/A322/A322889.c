/*
 * A322889  Chebyshev T-polynomials T_n(18).
 * https://oeis.org/A322889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=18, a(n)=36*a(n-1)-a(n-2) for n>1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)18;

    seq_t prev = (seq_t)1;
    seq_t curr = (seq_t)18;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = (seq_t)36 * curr - prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
