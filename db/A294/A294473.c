/*
 * A294473  Sum of the areas of the squares on the sides of the distinct
 *          rectangles that can be made with positive integer sides such that
 *          L + W = n, W < L.
 * https://oeis.org/A294473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n-3-(-1)^n)*(8*n-3+(-1)^n)/24
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 733, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute parity of n */
    idx_t odd = n & (idx_t)1;

    seq_t N   = (seq_t)n;

    /* t1 = 2*n - 3 - (-1)^n  and  t2 = 8*n - 3 + (-1)^n
       For odd n: (-1)^n = -1  → t1 = 2*n - 2,  t2 = 8*n - 4
       For even n: (-1)^n =  1  → t1 = 2*n - 4,  t2 = 8*n - 2 */
    seq_t t1 = odd ? (2*N - 2) : (2*N - 4);
    seq_t t2 = odd ? (8*N - 4) : (8*N - 2);

    seq_t result = N;
    result *= t1;
    result *= t2;
    result /= (seq_t)24;   /* division is exact */

    return result;
}
