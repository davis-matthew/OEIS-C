/*
 * A363256  Number of length n strings on the alphabet {0,1,2,3} with digit
 *          sum at most 4.
 * https://oeis.org/A363256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (((n + 10)*n + 35)*n + 26)*n/24 + 1
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t t = N + (seq_t)10;
    t *= N;
    t += (seq_t)35;
    t *= N;
    t += (seq_t)26;
    t *= N;
    t /= (seq_t)24;
    t += (seq_t)1;
    return t;
}
