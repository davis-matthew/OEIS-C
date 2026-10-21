/*
 * A290999  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - 6*S^2.
 * https://oeis.org/A290999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2) (a(0)=0, a(1)=6)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    seq_t prev = 0;   /* a(0) */
    seq_t curr = 6;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = 5 * prev + 2 * curr;
        prev = curr;
        curr = next;
    }
    return curr;
}
