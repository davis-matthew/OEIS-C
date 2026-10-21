/*
 * A260211  Irregular triangle read by rows, T(n,k) is the decimal number
 *          conversion from an n-bit symmetric binary table arranged in
 *          ascending order for n > 1.
 * https://oeis.org/A260211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the nth binary palindrome; all binary palindromes are
 *            listed by increasing bit‑length (starting at 1) and then in
 *            numeric order. The sequence is the concatenation of these
 *            palindromes
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* the sequence is defined for n ≥ 1 */

    idx_t total = 0;                /* terms before the current row */
    idx_t bits  = 1;                /* current row bit‑length */

    /* find the row (bits) that contains the nth term */
    for (;;) {
        idx_t half   = (bits + 1) / 2;               /* free bits in a palindrome */
        idx_t rowSize = ((idx_t)1) << half;         /* count of palindromes in this row */
        if (total + rowSize >= n) break;
        total += rowSize;
        ++bits;
    }

    /* index within the row (0‑based) */
    idx_t pos  = n - total - 1;
    idx_t half = (bits + 1) / 2;   /* free bits again */

    /* construct the palindrome from pos */
    seq_t val = 0;
    for (idx_t j = 0; j < bits; ++j) {
        idx_t bit;
        if (j < half) {
            bit = (pos >> (half - 1 - j)) & 1;
        } else {
            idx_t k = bits - 1 - j;               /* mirrored free‑bit index */
            bit = (pos >> (half - 1 - k)) & 1;
        }
        val = (val << 1) | bit;
    }
    return val;
}
