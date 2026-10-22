/*
 * A340528  Radio number of the path graph P_n.
 * https://oeis.org/A340528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(3)=3; for even n>1: a(n) = (n-2)*n/2 + 1; for odd n>1,
 *            n!=3: a(n) = (n-3)*(n+1)/2 + 4
 * Range:     u8 n <= 23, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;
    if (n == 3) return (seq_t)3;

    seq_t nn = (seq_t)n;

    if ((n & 1) == 0) {                /* even n */
        return ((nn * (nn - (seq_t)2)) / (seq_t)2) + (seq_t)1;
    } else {                            /* odd n (n != 3) */
        return (((nn - (seq_t)3) * (nn + (seq_t)1)) / (seq_t)2) + (seq_t)4;
    }
}
