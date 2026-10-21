/*
 * A250421  Number of length 4+1 0..n arrays with the sum of the minimum of
 *          each adjacent pair multiplied by some arrangement of +-1 equal to
 *          zero.
 * https://oeis.org/A250421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2/15)n^5 + (92/27)n^4 + (85/27)n^3 + (68/9)n^2 + g(n)*n
 *            + h(n) for n ≡ 0,1,2 (mod 3) where g(n) = 612/135, h(n) = 1 for
 *            n≡0 (mod 3) g(n) = 632/135, h(n) = 29/27 for n≡1 (mod 3) g(n) =
 *            652/135, h(n) = 31/27 for n≡2 (mod 3) These are equivalent to
 *            the piecewise polynomials given in the OEIS...
 * Range:     u8 none, u16 n <= 3, u32 n <= 68, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert n to seq_t for arithmetic */
    seq_t sn = (seq_t)n;

    /* compute powers of n */
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n3 = n2 * sn;          /* n^3 */
    seq_t n4 = n3 * sn;          /* n^4 */
    seq_t n5 = n4 * sn;          /* n^5 */

    seq_t num = 0;               /* numerator of a(n) over 135 */

    switch (n % 3)
    {
        case 0:  /* n ≡ 0 (mod 3) */
            num = ( (seq_t)18 * n5 ) +
                  ( (seq_t)460 * n4 ) +
                  ( (seq_t)425 * n3 ) +
                  ( (seq_t)1020 * n2 ) +
                  ( (seq_t)612 * sn ) +
                  ( (seq_t)135 ); /* constant term 1 = 135/135 */
            break;

        case 1:  /* n ≡ 1 (mod 3) */
            num = ( (seq_t)18 * n5 ) +
                  ( (seq_t)460 * n4 ) +
                  ( (seq_t)425 * n3 ) +
                  ( (seq_t)1020 * n2 ) +
                  ( (seq_t)632 * sn ) +
                  ( (seq_t)145 ); /* constant term 29/27 = 145/135 */
            break;

        case 2:  /* n ≡ 2 (mod 3) */
            num = ( (seq_t)18 * n5 ) +
                  ( (seq_t)460 * n4 ) +
                  ( (seq_t)425 * n3 ) +
                  ( (seq_t)1020 * n2 ) +
                  ( (seq_t)652 * sn ) +
                  ( (seq_t)155 ); /* constant term 31/27 = 155/135 */
            break;
    }

    /* denominator is 135 */
    return num / (seq_t)135;
}
