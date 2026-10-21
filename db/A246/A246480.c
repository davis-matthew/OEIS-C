/*
 * A246480  Number of length 1+3 0..n arrays with no pair in any consecutive
 *          four terms totalling exactly n.
 * https://oeis.org/A246480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = n^4 - 2*n^3 + 3*n^2 + 3*n - 3 for odd n * a(n) = n^4 -
 *            2*n^3 + 3*n^2 - n for even n * * (These are the exact closed
 *            formulas given in the OEIS entry.)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          // n
    seq_t n2 = sn * sn;           // n^2
    seq_t n3 = n2 * sn;           // n^3
    seq_t n4 = n3 * sn;           // n^4

    if (n & 1) {                  // odd n
        seq_t res   = n4;
        res += (seq_t)3 * n2;     // + 3*n^2
        res += (seq_t)3 * sn;     // + 3*n
        seq_t sub = (seq_t)2 * n3; // 2*n^3
        sub += (seq_t)3;          // + 3
        res -= sub;               // subtract (2*n^3 + 3)
        return res;
    } else {                      // even n
        seq_t res   = n4;
        res += (seq_t)3 * n2;     // + 3*n^2
        seq_t sub = sn;           // + n
        sub += (seq_t)2 * n3;     // + 2*n^3
        res -= sub;               // subtract (n + 2*n^3)
        return res;
    }
}
