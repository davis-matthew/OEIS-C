/*
 * A245954  Number of length 4+3 0..n arrays with some pair in every
 *          consecutive four terms totalling exactly n.
 * https://oeis.org/A245954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 48, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A245954
   Formula: 
     for n even: a(n) = 1 + 34*n + 6*n^2 - 16*n^3 + 66*n^4 + 15*n^5
     for n odd : a(n) = 58 + 57*n - 80*n^2 - 28*n^3 + 66*n^4 + 15*n^5
   (The two polynomials are equivalent to the empirical 10‑term recurrence.)
*/

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour for n<1 is irrelevant */

    const seq_t nn = (seq_t)n;      /* cast index to seq_t */
    const seq_t n2 = nn * nn;
    const seq_t n3 = n2 * nn;
    const seq_t n4 = n3 * nn;
    const seq_t n5 = n4 * nn;

    if ((n & 1) == 0) {            /* n is even */
        const seq_t pos = 1 + 34 * nn + 6 * n2 + 66 * n4 + 15 * n5;
        const seq_t neg = 16 * n3;
        return pos - neg;
    } else {                       /* n is odd */
        const seq_t pos = 58 + 57 * nn + 66 * n4 + 15 * n5;
        const seq_t neg = 80 * n2 + 28 * n3;
        return pos - neg;
    }
}
