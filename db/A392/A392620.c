/*
 * A392620  Number of maximal independent vertex sets in the n-necklace graph.
 * https://oeis.org/A392620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + 2*a(n-3) with initial values
 *            a(1)=4, a(2)=10, a(3)=34
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 1)  return 4;
    if (n == 2)  return 10;
    if (n == 3)  return 34;

    /* iterate forward using a rolling window */
    seq_t a1 = 4;   /* a(n-3) */
    seq_t a2 = 10;  /* a(n-2) */
    seq_t a3 = 34;  /* a(n-1) */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = 4 * a3 - 3 * a2 + 2 * a1;  /* compute a(i) */
        a1 = a2;  /* shift window */
        a2 = a3;
        a3 = an;
    }
    return an;   /* a(n) for n >= 4 */
}
