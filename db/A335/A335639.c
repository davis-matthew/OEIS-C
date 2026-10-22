/*
 * A335639  Sum of the positive differences of the cubed parts in each
 *          partition of n into two parts.
 * https://oeis.org/A335639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n-1)/2)} (n-i)^3 - i^3
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = (n - 1) / 2;
    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t ai = (seq_t)(n - i);
        seq_t bi = (seq_t)i;
        sum += ai * ai * ai - bi * bi * bi;
    }
    return sum;
}
