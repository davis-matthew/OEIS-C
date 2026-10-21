/*
 * A254948  Number of length 1 1..(n+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5, 7 or 11.
 * https://oeis.org/A254948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers k with 1 ≤ k ≤ n+1 such that k is
 *            divisible by 2, 3, 5, 7, or 11. Computed by inclusion–exclusion
 *            over the 5 primes {2,3,5,7,11}
 * Range:     u8 n <= 210, u16 n <= 210, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const idx_t primes[5] = {2,3,5,7,11};
    const idx_t P = 5;
    seq_t N   = (seq_t)n + 1;   /* count numbers up to n+1 */
    seq_t sum = 0;

    /* Iterate over all non‑empty subsets of the 5 primes. */
    for (idx_t mask = 1; mask < (1ULL << P); ++mask) {
        idx_t prod = 1;
        int bits = 0;
        for (int i = 0; i < P; ++i) {
            if (mask & (1ULL << i)) {
                prod *= primes[i];
                ++bits;
            }
        }
        seq_t term = N / prod;
        if (bits & 1)
            sum += term;   /* odd size → add */
        else
            sum -= term;   /* even size → subtract */
    }
    return sum;
}
