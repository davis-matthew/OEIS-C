/*
 * A267700  "Tree" sequence in a 90-degree sector of the cellular automaton of
 *          A160720.
 * https://oeis.org/A267700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (2^{popcount(k)} - 1)
 * Range:     u8 n <= 38, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        unsigned int pc = __builtin_popcountll((unsigned long long)i);
        seq_t diff = ((seq_t)1 << pc) - 1;
        sum += diff;
    }
    return sum;
}
