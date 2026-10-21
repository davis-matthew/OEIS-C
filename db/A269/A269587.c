/*
 * A269587  Number of length-7 0..n arrays with no repeated value differing
 *          from the previous repeated value by more than one.
 * https://oeis.org/A269587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + 7*n^6 + 11*n^5 + 35*n^4 + 28*n^3 + 42*n^2 + 5*n - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rn = (seq_t)n;
    seq_t res = rn;
    res = (res + (seq_t)7) * rn;
    res = (res + (seq_t)11) * rn;
    res = (res + (seq_t)35) * rn;
    res = (res + (seq_t)28) * rn;
    res = (res + (seq_t)42) * rn;
    res = (res + (seq_t)5) * rn;
    res = res - (seq_t)1;
    return res;
}
