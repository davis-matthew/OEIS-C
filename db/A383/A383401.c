/*
 * A383401  Index of the largest odd divisor in the list of divisors of n.
 * https://oeis.org/A383401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = index of the largest odd divisor in the list of divisors
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the largest odd divisor (odd part) of n */
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
    }

    /* Count how many divisors of n are strictly less than m */
    idx_t count = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            if (i < m) {
                ++count;
            }
            idx_t j = n / i;
            if (j != i && j < m) {
                ++count;
            }
        }
    }

    /* The desired position is one more than the number of smaller divisors */
    return (seq_t)(count + 1);
}
