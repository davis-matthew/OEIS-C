/*
 * A269691  Number of length-5 0..n arrays with no repeated value differing
 *          from the previous repeated value by plus or minus one modulo n+1.
 * https://oeis.org/A269691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + 5*n^4 + 10*n^3 + 4*n^2 - 3*n - 1 for n > 1 Special
 *            case: a(1) = 24 Implementation uses Horner's rule to avoid large
 *            intermediate values
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)24;
    }
    seq_t sn = n;                 /* Convert index to seq_t. */
    seq_t t1 = sn + (seq_t)5;
    seq_t t2 = sn * t1 + (seq_t)10;
    seq_t t3 = sn * t2 + (seq_t)4;
    seq_t t4 = sn * t3 - (seq_t)3;
    seq_t t5 = sn * t4 - (seq_t)1;
    return t5;
}
