/*
 * A342737  Number of wedged n-spheres in the homotopy type of the
 *          neighborhood complex of Kneser graph KG_{3,n}.
 * https://oeis.org/A342737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (n+1)*(n+3)*(n+4)*(n+6)/4
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t to avoid overflow of idx_t additions */
    seq_t n1 = (seq_t)n + (seq_t)1;   /* n+1 */
    seq_t n3 = (seq_t)n + (seq_t)3;   /* n+3 */
    seq_t n4 = (seq_t)n + (seq_t)4;   /* n+4 */
    seq_t n6 = (seq_t)n + (seq_t)6;   /* n+6 */

    seq_t prod;

    if ((n & (idx_t)1) == (idx_t)0) {
        /* n is even: n+4 and n+6 are even and contain all factors of 4 */
        seq_t half1 = n4 / (seq_t)2;   /* (n+4)/2 */
        seq_t half2 = n6 / (seq_t)2;   /* (n+6)/2 */

        prod = n1;
        prod *= n3;
        prod *= half1;
        prod *= half2;
    } else {
        /* n is odd: n+1 and n+3 are even and contain all factors of 4 */
        seq_t half1 = n1 / (seq_t)2;   /* (n+1)/2 */
        seq_t half2 = n3 / (seq_t)2;   /* (n+3)/2 */

        prod = half1;
        prod *= half2;
        prod *= n4;
        prod *= n6;
    }

    /* Add the += 1 as per the formula */
    return prod + (seq_t)1;
}
