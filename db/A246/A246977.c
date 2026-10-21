/*
 * A246977  Sequence B related to Fraenkel's (3,2)-Wythoff's game in Table 5
 *          of Liu-Zhao (2014).
 * https://oeis.org/A246977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=6*n for 0≤n≤5. For 6≤n≤30, a(n)=6*n+2*⌊(n-1)/5⌋. For n≥31,
 *            a(n)=6*n+2*⌊(n-3)/5⌋
 * Range:     u8 n <= 40, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first six terms are simple multiples of 6 */
    if (n <= 5u)                      /* n is unsigned, 5u is 5 */
        return (seq_t)n * 6;

    /* 6 <= n < 31 */
    if (n < 31u)
        return (seq_t)n * 6 + 2 *
               ( (seq_t)((n - 1) / 5) );

    /* n >= 31 */
    return (seq_t)n * 6 + 2 *
           ( (seq_t)((n - 3) / 5) );
}
