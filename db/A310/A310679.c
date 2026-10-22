/*
 * A310679  Coordination sequence Gal.6.42.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = 36·⌊n/10⌋ if n mod 10 == 0 a(n) =
 *            36·⌊n/10⌋ + 4 if n mod 10 == 1 a(n) = 32·⌊n/10⌋ + 3·(n mod 10) +
 *            1 otherwise This gives the exact coordination sequence of
 *            Gal.6.42.5
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t k = n / 10;
    idx_t r = n % 10;

    if (r == 0) {
        return (seq_t)36 * (seq_t)k;
    } else if (r == 1) {
        return (seq_t)36 * (seq_t)k + (seq_t)4;
    } else {
        return (seq_t)32 * (seq_t)k + (seq_t)3 * (seq_t)r + (seq_t)1;
    }
}
