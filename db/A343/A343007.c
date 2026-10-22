/*
 * A343007  Relative position of the average value between two consecutive
 *          partial sums of the Leibniz formula for Pi.
 * https://oeis.org/A343007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 + (-1)^(n+1) + 4*n + 4*n^2)/2
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternative: a(1)=6, a(n)=a(n-1)+ (4*n-(-1)^n) for n>1 */

seq_t a(idx_t n) {
    /* n is 1-based */
    seq_t nn = (seq_t)n;
    seq_t term = 4 * nn * nn + 4 * nn + 3;   /* 4*n^2 + 4*n + 3 */
    if (n & 1U)          /* n odd: add +1 */
        term += 1;
    else                 /* n even: subtract 1 */
        term -= 1;
    return term / 2;
}
