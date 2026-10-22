/*
 * A313116  Coordination sequence Gal.5.116.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n - s(n), * where s(n) = 2*(n/10) if n mod 10 = 0 * =
 *            2*(n/10)+1 otherwise * * This matches the OEIS sequence A313116
 *            (Coordination sequence Gal.5.116.1). * The derivation comes from
 *            observing that the difference pattern is * 9 times the same
 *            offset followed by a single step increment. *...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* All arithmetic is done in seq_t to keep the result exact. */
    if (n == 0) return (seq_t)1;

    seq_t k = (seq_t)n / (seq_t)10;   /* integer division */
    seq_t r = (seq_t)n % (seq_t)10;   /* remainder */

    seq_t s = (r == 0) ? (seq_t)2 * k : (seq_t)2 * k + (seq_t)1;

    return (seq_t)5 * (seq_t)n - s;
}
