/*
 * A268948  Number of length-7 0..n arrays with no repeated value unequal to
 *          the previous repeated value plus one mod n+1.
 * https://oeis.org/A268948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + 7*n^6 + 6*n^5 + 10*n^4 + 4*n^3 + n^2 + 4*n - 3
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The sequence is indexed starting at 1 (a(1) = 30). */

seq_t a(idx_t n)
{
    /* Cast to seq_t to avoid accidental 64‑bit overflow before
       the sequence type promotes it to the required precision. */
    seq_t x   = (seq_t)n;
    seq_t n2  = x * x;        /* n^2 */
    seq_t n3  = n2 * x;       /* n^3 */
    seq_t n4  = n3 * x;       /* n^4 */
    seq_t n5  = n4 * x;       /* n^5 */
    seq_t n6  = n5 * x;       /* n^6 */
    seq_t n7  = n6 * x;       /* n^7 */

    /* Compute the polynomial term by term. */
    seq_t result = n7;
    result += (seq_t)7 * n6;
    result += (seq_t)6 * n5;
    result += (seq_t)10 * n4;
    result += (seq_t)4 * n3;
    result += n2;
    result += (seq_t)4 * x;
    result -= (seq_t)3;

    return result;
}
