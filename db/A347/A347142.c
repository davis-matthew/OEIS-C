/*
 * A347142  Sum of 4th powers of divisors of n that are < sqrt(n).
 * https://oeis.org/A347142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n, d < sqrt(n)} d^4
 * Range:     u8 n <= 19, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d < n; ++d) {
        if (n % d == 0) {
            seq_t d4 = (seq_t)d * d * d * d;
            sum += d4;
        }
    }
    return sum;
}
