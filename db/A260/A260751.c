/*
 * A260751  25 primes in arithmetic progression: a(n) = 6171054912832631 +
 *          (n-1)*81737658082080 for n = 1, 2, ..., 25.
 * https://oeis.org/A260751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 6171054912832631 + (n-1)*81737658082080
 * Range:     u8 none, u16 none, u32 none, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t base = (seq_t)6171054912832631ULL;
    const seq_t diff = (seq_t)81737658082080ULL;
    seq_t idx = (seq_t)n;
    if (idx == 0) return base;               /* out-of-range guard (unused) */
    return base + (idx - 1) * diff;
}
