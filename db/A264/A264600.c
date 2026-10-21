/*
 * A264600  Let S_n denote the list of decimal numbers 0 to n, written
 *          backwards (allowing leading zeros) and arranged in lexicographic
 *          order; a(n) = position where backwards-n appears, starting
 *          indexing at 0.
 * https://oeis.org/A264600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(10*n + m) = a(n) + m*(n+1); a(0)=0;
 * Range:     u8 n <= 268, u16 n <= 9125, u32 n <= 9125, u64 n <= 9125,
 *            u128 n <= 9125, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 10)
        return (seq_t)n;
    idx_t q = n / 10;
    idx_t r = n % 10;
    return a(q) + (seq_t)r * ((seq_t)q + 1);
}
