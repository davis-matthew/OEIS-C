/*
 * A347172  Sum of 4th powers of odd divisors of n that are < sqrt(n).
 * https://oeis.org/A347172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n, d odd, d<sqrt(n)} d^4
 * Range:     u8 n <= 29, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d < n / d; ++d) {
        if ((n % d) == 0 && (d & 1u)) {
            seq_t d2 = (seq_t)d * (seq_t)d;
            seq_t d4 = d2 * d2;
            sum += d4;
        }
    }
    return sum;
}
