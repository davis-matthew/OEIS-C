/*
 * A359495  Sum of positions of 1's in binary expansion minus sum of positions
 *          of 1's in reversed binary expansion, where positions in a sequence
 *          are read starting with 1 from the left.
 * https://oeis.org/A359495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, base
 * Formula:   a(n) = Σ_{j=1}^{k} (k + 1 - 2*j) * ((n >> (j-1)) & 1) where k =
 *            ⌊log₂(n)⌋ + 1 (k = 0 for n = 0)
 * Range:     i8 n <= 8982, i16 n <= 8982, i32 n <= 8982, i64 n <= 8982,
 *            i128 n <= 8982, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* determine number of binary digits k */
    idx_t k = 0;
    idx_t tmp = n;
    while (tmp) {
        ++k;
        tmp >>= 1;
    }

    seq_t sum = 0;
    for (idx_t j = 1; j <= k; ++j) {
        if ((n >> (j - 1)) & 1) {
            seq_t coef = (seq_t)(k + 1) - (seq_t)(2 * j);
            sum += coef;
        }
    }
    return sum;
}
