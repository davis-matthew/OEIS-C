/*
 * A242111  Number of nonnegative k such that k^2 + 2 divides n^2 + 2.
 * https://oeis.org/A242111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of integers k ≥ 0 such that (k² + 2)
 *            divides (n² + 2)
 * Range:     u8 n <= 15, u16 n <= 1396, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n² + 2 once */
    seq_t val = (seq_t)n * (seq_t)n + 2;
    seq_t count = 0;

    /* k must satisfy k² + 2 ≤ n² + 2, hence k ≤ n */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t d = (seq_t)k * (seq_t)k + 2;
        if (val % d == 0) {
            ++count;
        }
    }

    return count;
}
