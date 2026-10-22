/*
 * A343182  Binary word formed from first 2^n-1 terms of paper-folding
 *          sequence A014577, reversed and complemented.
 * https://oeis.org/A343182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal of the reverse‑complement of the first 2^(n+1)-1
 *            terms of the paper‑folding sequence A014577
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute m = 2^(n+1) - 1 without overflowing shift operators */
    idx_t m = 0;
    for (idx_t i = 0; i <= n; ++i)
        m = (m << 1) | 1;          /* after loop m = 2^(n+1) - 1 */

    seq_t result = 0;

    for (idx_t k = m; k >= 1; --k) {
        /* paper‑folding bit for position k (1‑based) */
        idx_t lsb     = k & (~k + 1);   /* lowest set bit of k = k & -k */
        idx_t shifted = lsb << 1;
        seq_t bit = ((shifted & k) == 0) ? (seq_t)1 : (seq_t)0;
        seq_t comp = (seq_t)1 - bit;    /* reverse complement */

        /* build decimal value from digits 0/1 */
        result = result * (seq_t)10 + comp;
    }

    return result;
}
