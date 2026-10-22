/*
 * A334559  a(2n+1) = (2n)^2 - 1, a(4n+2) = (4n+2)^2 - 3, a(4n) = (4n)^2 - 5,
 *          except for a(1) = 1, a(4) = 2. (Conjectured outcome of an
 *          elimination game, see comments.)
 * https://oeis.org/A334559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n+1) = (2n)^2-1, a(4n+2) = (4n+2)^2-3, a(4n) = (4n)^2-5
 *            except a(1)=1 and a(4)=2
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle special cases */
    if (n == 1) return (seq_t)1;
    if (n == 4) return (seq_t)2;

    /* For odd indices: n = 2k+1, k >= 1 */
    if (n & 1) {
        seq_t m = (seq_t)(n - 1);   /* m = 2k */
        return m * m - (seq_t)1;
    }

    /* For even indices */
    if ((n >> 1) & 1) {   /* n % 4 == 2 */
        seq_t m = (seq_t)n;
        return m * m - (seq_t)3;
    }

    /* Otherwise n % 4 == 0 */
    seq_t m = (seq_t)n;
    return m * m - (seq_t)5;
}
