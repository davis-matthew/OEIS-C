/*
 * A359938  Number of divisors d of n such that d-1 is square.
 * https://oeis.org/A359938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{d | n : d-1 is a perfect square} Implementation: For
 *            each m ≥ 0 with m²+1 ≤ n, check if (m²+1) divides n. Count the
 *            number of such divisors
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t m = 0; ; ++m)
    {
        idx_t k = m * m + 1;
        if (k > n) break;
        if (n % k == 0u) ++count;
    }
    return count;
}
