/*
 * A285998  a(n) = Sum_{k=0..floor(n/2)} (n-k)*(k+1).
 * https://oeis.org/A285998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (n-k)*(k+1)
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Guard for the offset (a(1) is the first term) */
    if (n == 0) return 0;

    seq_t nn = n;
    seq_t m  = nn >> 1;                     /* floor(n/2) */
    seq_t term1 = nn * (m + 1) * (m + 2);    /* n*(m+1)*(m+2)  */
    term1 = term1 / 2;                       /* /2  */
    seq_t term2 = m * (m + 1) * (m + 2);     /* m*(m+1)*(m+2)  */
    term2 = term2 / 3;                       /* /3  */
    return term1 - term2;
}
