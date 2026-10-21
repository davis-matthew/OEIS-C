/*
 * A269622  Number of length-6 0..n arrays with no repeated value differing
 *          from the previous repeated value by other than plus two, zero or
 *          minus 1.
 * https://oeis.org/A269622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=51, a(2)=624, a(n) = n^6 + 6*n^5 + 9*n^4 + 22*n^3 + 9*n^2 +
 *            9*n - 7 for n>2
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 51;
    if (n == 2)
        return 624;

    /* work in seq_t to allow arbitrary‑precision calculation */
    seq_t nn = (seq_t)n;

    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */
    seq_t n5 = n4 * nn;          /* n^5 */
    seq_t n6 = n5 * nn;          /* n^6 */

    seq_t result = n6           /* n^6 */
                 + (6 * n5)     /* 6*n^5 */
                 + (9 * n4)     /* 9*n^4 */
                 + (22 * n3)    /* 22*n^3 */
                 + (9 * n2)     /* 9*n^2 */
                 + (9 * nn);    /* 9*n */

    return result - 7;
}
