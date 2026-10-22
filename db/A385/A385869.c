/*
 * A385869  The maximum possible number of 7-cycles in an outerplanar graph on
 *          n vertices.
 * https://oeis.org/A385869
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*floor(n/4) - 36,33,28,23 depending on n mod 4
 *            (a(7)=1). This implements the piecewise linear formula given in
 *            the OEIS entry
 * Range:     u8 n <= 58, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset is 7, a(7)=1 */
    if (n == 7) return 1;
    if (n < 7) return 0;  /* undefined below offset */

    idx_t m = n / 4;          /* floor(n/4) */
    idx_t r = n % 4;          /* remainder */
    idx_t val = 0;

    switch (r) {
        case 0: val = m * 20 - 36; break;
        case 1: val = m * 20 - 33; break;
        case 2: val = m * 20 - 28; break;
        case 3: val = m * 20 - 23; break;
    }
    return (seq_t)val;
}
