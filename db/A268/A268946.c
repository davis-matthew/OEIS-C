/*
 * A268946  Number of length-5 0..n arrays with no repeated value unequal to
 *          the previous repeated value plus one mod n+1.
 * https://oeis.org/A268946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + 5*n^4 + 4*n^3 + 3*n^2 + 2*n - 1
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t r  = (seq_t)1;
    r = r * nn + (seq_t)5;  // r = n + 5
    r = r * nn + (seq_t)4;  // r = n^2 + 5n + 4
    r = r * nn + (seq_t)3;  // r = n^3 + 5n^2 + 4n + 3
    r = r * nn + (seq_t)2;  // r = n^4 + 5n^3 + 4n^2 + 3n + 2
    r = r * nn - (seq_t)1;  // r = n^5 + 5n^4 + 4n^3 + 3n^2 + 2n - 1
    return r;
}
