/*
 * A269498  Number of length-7 0..n arrays with no repeated value differing
 *          from the previous repeated value by one.
 * https://oeis.org/A269498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + 7*n^6 + 21*n^5 + 15*n^4 + 7*n^3 + 17*n^2 - n - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (offset 1: a(1) = 66) */

seq_t a(idx_t n)
{
    /* If n = 0 the sequence is undefined; return 0 to avoid underflow. */
    if (n == 0)
        return 0;

    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;           /* n^2 */
    seq_t n3  = n2 * N;          /* n^3 */
    seq_t n4  = n3 * N;          /* n^4 */
    seq_t n5  = n4 * N;          /* n^5 */
    seq_t n6  = n5 * N;          /* n^6 */
    seq_t n7  = n6 * N;          /* n^7 */

    seq_t result = n7;
    result += 7 * n6;
    result += 21 * n5;
    result += 15 * n4;
    result += 7 * n3;
    result += 17 * n2;
    result -= N;
    result -= 1;

    return result;
}
