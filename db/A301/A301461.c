/*
 * A301461  Number of integers less than or equal to n whose largest prime
 *          factor is 3.
 * https://oeis.org/A301461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers k ≤ n such that the largest prime
 *            factor of k is 3. The numbers with largest prime factor 3 are
 *            exactly those of the form 2^a · 3^b with a ≥ 0, b ≥ 1. For each
 *            b, the admissible a satisfy 2^a ≤ n / 3^b, so there are
 *            floor(log₂(n/3^b)) + 1 possibilities. The function sums...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* iterate over b: 3^b ≤ n */
    idx_t power3 = 3;
    while (power3 <= n) {
        idx_t limit = n / power3;          /* maximum 2^a allowed */
        /* count powers of 2 not exceeding limit */
        idx_t power2 = 1;
        seq_t count = 0;
        while (power2 <= limit) {
            count++;
            /* avoid overflow when shifting power2 */
            if (power2 > limit / 2) break;
            power2 <<= 1;
        }
        result += count;

        /* advance to next power of 3, guarding against overflow */
        if (power3 > n / 3) break;
        power3 *= 3;
    }
    return result;
}
