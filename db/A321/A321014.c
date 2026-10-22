/*
 * A321014  Number of divisors of n which are greater than 3.
 * https://oeis.org/A321014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|n, d>3} 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* smallest divisor > 3 is 4, so all n < 4 give 0 */

    seq_t count = 0;
    /* iterate over possible divisor pairs (i, n/i) */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            if (i > 3) ++count;           /* divisor i itself */
            idx_t j = n / i;
            if (j != i && j > 3) ++count; /* paired divisor j */
        }
    }
    return count;
}
