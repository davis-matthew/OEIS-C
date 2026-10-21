/*
 * A297024  Sum of the smaller parts of the partitions of n into two parts
 *          such that the smaller part does not divide the larger.
 * https://oeis.org/A297024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} i * (1 -
 *            (floor(n/i)-floor((n-1)/i)))
 * Range:     u8 n <= 46, u16 n <= 726, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = Σ_{i=1}^{⌊n/2⌋} i   if i does not divide n , else 0   */

seq_t a(idx_t n)
{
    /* Compute m = floor(n/2) */
    idx_t m = n / 2;

    /* Total sum of integers from 1 to m */
    seq_t total = (seq_t)m * ((seq_t)m + 1) / 2;

    /* Subtract all divisors of n that are ≤ m */
    for (idx_t i = 1; i <= m; ++i) {
        if (n % i == 0) {
            total -= (seq_t)i;
        }
    }
    return total;
}
