/*
 * A322462  Numbers on the 0-1-12 line in a spiral on a grid of equilateral
 *          triangles.
 * https://oeis.org/A322462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(n+2)/2 for even n, a(n) = a(n-1)+1 for odd n
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & (idx_t)1) == 0) {          /* even index */
        seq_t tn = (seq_t)n;
        seq_t res = tn * (tn + 2);
        res /= 2;
        res *= 3;
        return res;
    } else {                            /* odd index */
        idx_t m = n - 1;                /* m is even */
        seq_t tm = (seq_t)m;
        seq_t res = tm * (tm + 2);
        res /= 2;
        res *= 3;
        res += 1;
        return res;
    }
}
