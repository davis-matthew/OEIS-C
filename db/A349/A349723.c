/*
 * A349723  Atomic number corresponding to the element that is the first of
 *          the two middle elements in the n-th row of the periodic table of
 *          elements.
 * https://oeis.org/A349723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 18*n^2 + 32*n + 3*(-1)^n - 27)/24
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 1038, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* seq_t is an unsigned integer type; all intermediate values fit in it for n ≥ 1 */
    seq_t nn   = (seq_t)n;                 /* n */
    seq_t n2   = nn * nn;                  /* n^2 */
    seq_t n3   = n2 * nn;                  /* n^3 */

    seq_t term1 = 4 * n3;                  /* 4*n^3 */
    seq_t term2 = 18 * n2;                 /* 18*n^2 */
    seq_t term3 = 32 * nn;                 /* 32*n */

    /* add the sign term 3*(-1)^n */
    seq_t numerator = term1 + term2 + term3;
    if ((n & 1) == 0) {                   /* n even → (-1)^n = 1, add +3 */
        numerator += 3;
    } else {                              /* n odd  → (-1)^n = -1, subtract 3 */
        numerator -= 3;
    }

    numerator -= 27;                      /* subtract 27 */

    return numerator / 24;                /* integer division */
}
