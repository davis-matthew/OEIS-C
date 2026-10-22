/*
 * A306185  a(n) = (2n+1)!! + (2n)!! where k!! = A006882(k).
 * https://oeis.org/A306185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n+1)!! + (2n)!!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* out of range for offset 1 */
    seq_t even = 1;
    seq_t odd  = 1;
    for (idx_t i = 1; i <= n; ++i) {
        even *= ((seq_t)2 * i);            /* multiply by 2i          */
        odd  *= ((seq_t)2 * i) - 1;        /* multiply by (2i-1)      */
    }
    odd *= ((seq_t)2 * n) + 1;              /* include the last odd (2n+1) */
    return even + odd;
}
