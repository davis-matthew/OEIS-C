/*
 * A279447  Number of nonequivalent ways to place 3 points on an n X n square
 *          grid so that no more than 2 points are on a vertical or horizontal
 *          straight line.
 * https://oeis.org/A279447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 - 5*n^4 + 14*n^3 - 14*n^2 + 4*n)/48 + IF(n is odd,
 *            (2*n^3 - 3*n^2 + 1)/16, 0) The formula is evaluated by combining
 *            the two fractions to a common denominator 48: a(n) = [ n^6 -
 *            5*n^4 + 14*n^3 - 14*n^2 + 4*n + 3*(2*n^3 - 3*n^2 + 1) * I(n odd)
 *            ] / 48 where I(n odd) is 1 when n is odd an...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 52, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n < 1 (offset starts at 1). */
    if (n == 0) return 0;

    const seq_t ONE   = (seq_t)1;
    const seq_t TWO   = (seq_t)2;
    const seq_t THREE = (seq_t)3;
    const seq_t FOUR  = (seq_t)4;
    const seq_t FIVE  = (seq_t)5;
    const seq_t FOURTEEN = (seq_t)14;
    const seq_t FORTY_EIGHT = (seq_t)48;

    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;          /* n^2  */
    seq_t n3  = n2 * N;         /* n^3  */
    seq_t n4  = n3 * N;         /* n^4  */
    seq_t n6  = n4 * n2;        /* n^6  */

    /* Base numerator: n^6 - 5*n^4 + 14*n^3 - 14*n^2 + 4*n */
    seq_t base = n6
                - FIVE * n4
                + FOURTEEN * n3
                - FOURTEEN * n2
                + FOUR * N;

    /* If n is odd, add the extra term (2*n^3 - 3*n^2 + 1)*3 to bring it
       to a common denominator of 48. */
    seq_t extra = 0;
    if ((n & 1ULL) != 0) {
        seq_t term = TWO * n3 - THREE * n2 + ONE;   /* 2*n^3 - 3*n^2 + 1 */
        extra = term * THREE;                       /* multiply by 3 */
    }

    /* Final result: (base + extra) / 48 */
    return (base + extra) / FORTY_EIGHT;
}
