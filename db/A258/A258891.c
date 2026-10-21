/*
 * A258891  Number of (n+2) X (5+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum less than the antidiagonal sum or central row sum
 *          less than the central column sum.
 * https://oeis.org/A258891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + 10*n^3 + 149*n^2 + 620*n + 3844 for even n≥4, a(n)
 *            = n^4 + 10*n^3 + 150*n^2 + 610*n + 3869 for odd n≥4. a(1)=30870,
 *            a(2)=23409, a(3)=12804
 * Range:     u8 none, u16 n <= 11, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 30870;
    if (n == 2)
        return 23409;
    if (n == 3)
        return 12804;

    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;
    seq_t n4 = n3 * ns;

    if ((n & 1) == 0) {           /* even n */
        return n4 + (seq_t)10 * n3 + (seq_t)149 * n2 + (seq_t)620 * ns + (seq_t)3844;
    } else {                      /* odd n */
        return n4 + (seq_t)10 * n3 + (seq_t)150 * n2 + (seq_t)610 * ns + (seq_t)3869;
    }
}
