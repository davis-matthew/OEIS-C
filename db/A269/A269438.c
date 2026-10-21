/*
 * A269438  Number of length-6 0..n arrays with no repeated value greater than
 *          the previous repeated value.
 * https://oeis.org/A269438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*n^6 + 36*n^5 + 72*n^4 + 88*n^3 + 69*n^2 + 29*n + 6)/6
 * Range:     u8 none, u16 n <= 3, u32 n <= 38, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the closed-form polynomial for A269438. */

seq_t a(idx_t n)
{
    /* Cast n only when it is used in arithmetic */
    seq_t v = (seq_t)6;                 /* 6 */
    v = v * (seq_t)n + (seq_t)36;       /* 6*n + 36 */
    v = v * (seq_t)n + (seq_t)72;       /* (6*n+36)*n + 72 */
    v = v * (seq_t)n + (seq_t)88;       /* ((6*n+36)*n + 72)*n + 88 */
    v = v * (seq_t)n + (seq_t)69;       /* ... + 69 */
    v = v * (seq_t)n + (seq_t)29;       /* ... + 29 */
    v = v * (seq_t)n + (seq_t)6;        /* ... + 6 (numerator) */
    return v / (seq_t)6;                /* divide by 6 to obtain a(n) */
}
