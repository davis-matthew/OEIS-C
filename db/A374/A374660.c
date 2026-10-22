/*
 * A374660  a(n) = Product_{k=0..n+1} L(k)+4, where L=A000032 (Lucas numbers).
 * https://oeis.org/A374660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0}^{n} (L(k)+4) (L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t L0 = (seq_t)2;
    seq_t L1 = (seq_t)1;
    result = L0 + (seq_t)4;          /* term for k = 0 */
    seq_t prev = L0;
    seq_t curr = L1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = curr + (seq_t)4;
        result *= term;
        seq_t next = prev + curr;    /* next Lucas number */
        prev = curr;
        curr = next;
    }
    return result;
}
