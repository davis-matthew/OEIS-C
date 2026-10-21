/*
 * A263660  Number of length n arrays of permutations of 0..n-1 with each
 *          element moved by -2 to 2 places and with no two consecutive
 *          increases or two consecutive decreases.
 * https://oeis.org/A263660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=4, a(4)=7; for n>=5: if even
 *            a(n)=3*2^(n/2-1); if odd a(n)=2^((n-5)/2+3)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)7;

    if ((n & 1) == 0) {        /* even n */
        idx_t exp = n / 2 - 1;
        seq_t res = (seq_t)1;
        for (idx_t i = 0; i < exp; ++i) res *= 2;
        res *= 3;
        return res;
    } else {                   /* odd n */
        idx_t exp = (n - 5) / 2 + 3;
        seq_t res = (seq_t)1;
        for (idx_t i = 0; i < exp; ++i) res *= 2;
        return res;
    }
}
