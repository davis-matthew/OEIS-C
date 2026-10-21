/*
 * A288962  Number of 4-cycles in the n X n rook graph.
 * https://oeis.org/A288962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*binomial(n,2)*(n^2-4*n+5)/2
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (not used in this implementation):
   a(n) = 6*a(n-1)-15*a(n-2)+20*a(n-3)-15*a(n-4)+6*a(n-5)-a(n-6),
   with initial values a(1)=0,a(2)=1,a(3)=9,a(4)=60,a(5)=250,a(6)=765 */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* binomial(n,2) = n*(n-1)/2 */
    seq_t bin = nn * (nn - (seq_t)1) / (seq_t)2;
    /* part = n^2 - 4*n + 5 */
    seq_t part = nn * nn - (seq_t)4 * nn + (seq_t)5;
    seq_t result = nn * bin * part / (seq_t)2;
    return result;
}
