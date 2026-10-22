/*
 * A363980  Tom Greer's arithmetic progression of 27 primes.
 * https://oeis.org/A363980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 277699295941594831 + (n-1)*38110169025918990
 * Range:     u8 none, u16 none, u32 none, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t base = (seq_t)277699295941594831ULL;
    const seq_t step = (seq_t)38110169025918990ULL;
    return base + ((seq_t)(n - 1) * step);
}
