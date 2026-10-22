/*
 * A347611  a(n) is the n-th n-factorial number: a(n) = n!_n.
 * https://oeis.org/A347611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{j=1..n} (n^j-1)/(n-1) (n>1, a(0)=a(1)=1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    seq_t result = 1;
    seq_t base   = (seq_t)n;
    seq_t denom  = base - 1;
    seq_t pow    = base;                 /* n^1 */

    for (idx_t j = 1; j <= n; ++j) {
        seq_t term = (pow - 1) / denom;  /* (n^j - 1)/(n-1) */
        result *= term;
        pow *= base;                     /* advance to n^{j+1} */
    }

    return result;
}
