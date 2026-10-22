/*
 * A338021  Number of partitions of n into two parts (s,t) such that s <= t
 *          and t | s*n.
 * https://oeis.org/A338021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(s,t): s+t=n, s≤t, t|(s*n)}
 * Range:     u8 n <= 23, u16 n <= 5494, u32 n <= 9381, u64 n <= 9381,
 *            u128 n <= 9381, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by iterating over s=1..⌊n/2⌋ and counting valid t=n-s */
seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t limit = n / 2;
    for (idx_t s = 1; s <= limit; ++s) {
        idx_t t = n - s;
        seq_t prod = (seq_t)s * (seq_t)n;
        if (prod % (seq_t)t == 0) {
            ++count;
        }
    }
    return count;
}
