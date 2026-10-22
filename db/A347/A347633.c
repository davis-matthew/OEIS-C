/*
 * A347633  Number of minimum dominating sets in the path graph P_n.
 * https://oeis.org/A347633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, n = 0 (mod 3); a(n) = (n^2 + 13n + 4)/18, n = 1 (mod
 *            3); a(n) = (n + 4)/3, n = 2 (mod 3)
 * Range:     u8 n <= 12, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) is defined as 1 for completeness */
        return (seq_t)1;

    seq_t nseq = (seq_t)n;
    idx_t mod = n % 3;

    if (mod == 0)
        return (seq_t)1;

    if (mod == 1) {
        seq_t num = nseq * nseq + (seq_t)13 * nseq + (seq_t)4;
        return num / (seq_t)18;
    }

    /* mod == 2 */
    return (nseq + (seq_t)4) / (seq_t)3;
}
