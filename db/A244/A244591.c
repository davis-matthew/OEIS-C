/*
 * A244591  Zero followed by the terms of A032924 arranged to give the unique
 *          path to the n-th node of a complete, rooted and ordered binary
 *          tree.
 * https://oeis.org/A244591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>1, let k = ⌊log2 n⌋. Then a(n) = Σ_{i=0}^{k-1} (1
 *            + ((n >> i) & 1)) · 3^i (i.e. interpret the binary digits of n
 *            after the leading 1 as base‑3 digits 1 (left) and 2 (right),
 *            with the first step being the least significant digit)
 * Range:     u8 n <= 63, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1)
        return (seq_t)0;

    /* compute k = floor(log2 n) */
    idx_t m = n;
    unsigned k = 0;
    while (m > 1) {
        m >>= 1;
        k++;
    }

    seq_t num = 0;
    for (unsigned i = 0; i < k; i++) {
        int bit = (int)((n >> i) & 1);
        num = num * (seq_t)3 + (seq_t)(1 + bit);
    }
    return num;
}
