/*
 * A262376  a(n) = Sum_{k=0..n} (k! - k).
 * https://oeis.org/A262376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (k! - k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum  = 0;
    seq_t fact = 1;   /* fact holds k! at the start of each iteration,
                         starting with 0! for k = 0 */

    /* k = 0 term: factorial is 1, subtract 0 */
    sum += fact;

    for (idx_t k = 1; k <= n; ++k) {
        fact *= (seq_t)k;                /* now fact = k!   */
        sum += fact - (seq_t)k;          /* add (k! - k)   */
    }

    return sum;
}
