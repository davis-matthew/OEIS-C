/*
 * A357130  a(n) = 2*n - (-1)^n*(1+(n mod 2)).
 * https://oeis.org/A357130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - (-1)^n*(1+(n mod 2))
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = 2*n + 3*(n mod 2) - 1    */
/* For n odd: a(n) = 2*n + 2, for n even: a(n) = 2*n - 1 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;          /* convert index to seq_t */
    if (n & 1u) {                /* n is odd */
        return (seq_t)( (seq_t)2 * x + (seq_t)2 );
    } else {                     /* n is even */
        return (seq_t)( (seq_t)2 * x - (seq_t)1 );
    }
}
