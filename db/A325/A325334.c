/*
 * A325334  Number of integer partitions of n with adjusted frequency depth 3
 *          whose parts cover an initial interval of positive integers.
 * https://oeis.org/A325334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k >= 2 | n divisible by k(k+1)/2 }
 * Range:     u8 n <= 380, u16 n <= 8924, u32 n <= 8924, u64 n <= 8924,
 *            u128 n <= 8924, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 2; ; ++k) {
        seq_t tk = (seq_t)k * ((seq_t)k + 1) / 2;
        if (tk > (seq_t)n) break;
        if ((seq_t)n % tk == 0) ++count;
    }
    return count;
}
