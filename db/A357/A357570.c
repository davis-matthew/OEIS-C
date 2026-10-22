/*
 * A357570  a(n) = Sum_{k=0..floor(n/5)} (n-4*k)!/(n-5*k)!.
 * https://oeis.org/A357570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/5⌋} (n-4*k)!/(n-5*k)!
 * Range:     u8 n <= 18, u16 n <= 30, u32 n <= 50, u64 n <= 85, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* term for k = 0 is always 1 */
    seq_t sum = (seq_t)1;
    for (idx_t k = 1; k <= n / 5; ++k) {
        seq_t term = (seq_t)1;
        idx_t start = n - 5 * k + 1;   /* first factor in the product */
        idx_t end   = n - 4 * k;       /* last factor in the product */
        for (idx_t i = start; i <= end; ++i) {
            term *= (seq_t)i;
        }
        sum += term;
    }
    return sum;
}
