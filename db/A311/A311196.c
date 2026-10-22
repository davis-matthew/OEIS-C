/*
 * A311196  Coordination sequence Gal.6.115.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n >= 1: a(n) = 4*n + C(n) where C(n) =
 *            floor((n-6)/12) + 1 if n >= 6 + floor((n-7)/12) + 1 if n >= 7
 *            (C(n) counts how many times the increment 5 occurs up to index
 *            n.) This gives the sequence
 *            1,4,8,12,16,20,25,30,34,38,42,46,50,54,58,62,66,70,75,80,84,88,92,96,
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t value = (seq_t)n * 4;

    if (n >= 6) {
        value += ((seq_t)(n - 6) / 12) + 1;
    }
    if (n >= 7) {
        value += ((seq_t)(n - 7) / 12) + 1;
    }

    return value;
}
