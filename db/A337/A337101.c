/*
 * A337101  Number of partitions of n into two positive parts (s,t), s <= t,
 *          such that the harmonic mean of s and t is an integer.
 * https://oeis.org/A337101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ i | 1 ≤ i ≤ ⌊n/2⌋ and n divides 2*i*(n-i) }
 * Range:     u8 n <= 23, u16 n <= 524, u32 n <= 9412, u64 n <= 9412,
 *            u128 n <= 9412, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* safety for n=0, although offset is 1 */
    idx_t limit = n / 2;
    seq_t count = 0;
    const seq_t sn = (seq_t)n;       /* avoid repeated casts */

    for (idx_t i = 1; i <= limit; ++i) {
        seq_t prod = (seq_t)2 * i * (seq_t)(n - i);
        if (prod % sn == 0) ++count;
    }
    return count;
}
