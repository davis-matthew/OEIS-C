/*
 * A292346  The forgotten topological index of the Aztec diamond AZ(n) (see
 *          the Ramanes et al. reference, Theorem 2.1).
 * https://oeis.org/A292346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 128*n^2 + 160*n - 84 (offset 1)
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined for offset 1, safe fallback */
    seq_t sn = (seq_t)n;                 /* convert index to seq_t */
    seq_t term = 128 * sn * sn;          /* 128 * n^2 */
    term += 160 * sn;                    /* + 160 * n */
    term -= 84;                          /* - 84 */
    return term;
}
