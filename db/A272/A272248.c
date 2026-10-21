/*
 * A272248  a(n) = Product_{k=0..n} (n^5 + k^5).
 * https://oeis.org/A272248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=0..n} (n^5 + k^5)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A272248.
 * Uses only integer arithmetic on the class/typedef seq_t. */
seq_t a(idx_t n)
{
    seq_t result = 1;                   /* multiplication identity   */
    seq_t n5, k5;
    idx_t k;
    /* compute n^5 once */
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;                 /* n^2  */
    seq_t n3 = n2 * ns;                 /* n^3  */
    seq_t n4 = n3 * ns;                 /* n^4  */
    n5 = n4 * ns;                       /* n^5  */

    /* iterate over k = 0 .. n */
    for (k = 0; ; ++k)
    {
        seq_t ks = (seq_t)k;
        seq_t k2 = ks * ks;             /* k^2  */
        seq_t k3 = k2 * ks;             /* k^3  */
        seq_t k4 = k3 * ks;             /* k^4  */
        k5 = k4 * ks;                   /* k^5  */
        result *= (n5 + k5);            /* multiply term   */
        if (k == n)
            break;                      /* loop ends at k = n */
    }
    return result;
}
