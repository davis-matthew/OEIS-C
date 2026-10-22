/*
 * A352033  Sum of the 5th powers of the odd proper divisors of n.
 * https://oeis.org/A352033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d<n, d odd} d^5
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 169, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Iterate over odd divisors less than n */
    for (idx_t d = 1; d < n; d += 2) {
        if (n % d == 0) {
            seq_t dd = (seq_t)d;
            seq_t d5 = dd;
            d5 *= dd;
            d5 *= dd;
            d5 *= dd;
            d5 *= dd;           /* dd^5 */
            sum += d5;
        }
    }
    return sum;
}
