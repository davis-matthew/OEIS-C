/*
 * A337957  Number of unoriented colorings of the 8 cubic facets of a
 *          tesseract or of the 8 vertices of a hyperoctahedron.
 * https://oeis.org/A337957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial( binomial(n+1,2)+3 , 4)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = binomial(m,4) where m = n*(n+1)/2 + 3 */

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* index starts at 1 in OEIS */

    /* Convert the index to seq_t for integer arithmetic */
    seq_t nn = (seq_t)n;

    /* m = n*(n+1)/2 + 3 */
    seq_t m = nn * (nn + 1) / 2 + 3;

    /* Compute binomial(m,4) iteratively to keep intermediate results small */
    seq_t res = 1;
    for (int i = 0; i < 4; ++i) {
        /* res = res * (m - i) / (i + 1) */
        res = res * (m - i) / (i + 1);
    }
    return res;
}
