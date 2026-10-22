/*
 * A362872  Length of the "fractional part" of the phi-representation of n.
 * https://oeis.org/A362872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * #{ i ≥ 0 : L_{2i+1} ≤ n-1 } where L_k is the Lucas
 *            sequence defined by L_0 = 2, L_1 = 1 and L_{k} = L_{k-1} +
 *            L_{k-2}. The jumps in a(n) occur exactly at n = L_{2i+1} + 1 for
 *            i ≥ 0. This implementation computes the count directly by
 *            generating Lucas numbers up to n-1
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t m = (seq_t)(n - 1);   /* we need to compare with n-1 */
    seq_t L0 = 2;               /* L_0 */
    seq_t L1 = 1;               /* L_1 */
    seq_t count = 0;            /* number of odd-index Lucas numbers ≤ m */

    /* L_1 is odd index */
    if (L1 <= m) count = count + 1;

    seq_t Lk;
    idx_t k = 2;
    for (;;) {
        Lk = L1 + L0;           /* L_k = L_{k-1} + L_{k-2} */
        if (Lk > m) break;
        if (k & 1) count = count + 1;   /* odd index check */
        L0 = L1;
        L1 = Lk;
        ++k;
    }

    return count * 2;           /* each jump contributes 2 */
}
