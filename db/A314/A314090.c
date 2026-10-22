/*
 * A314090  Coordination sequence Gal.4.54.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   For n = 0 → a(0) = 1 For n > 0, let k = n / 4, r = n % 4. a(4k)
 *            = 20k a(4k+1) = 20k + 5 – (k mod 2) a(4k+2) = 20k + 11 – 2·(k
 *            mod 2) a(4k+3) = 20k + 16 – (k mod 2) (All operations are
 *            performed in seq_t.)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t k = n / 4;
    idx_t r = n % 4;
    seq_t base = (seq_t)20 * (seq_t)k;
    seq_t s = k & 1;   /* k mod 2 */

    switch (r) {
        case 0:
            return base;
        case 1:
            return base + (seq_t)5 - s;
        case 2:
            return base + (seq_t)11 - (seq_t)(2 * s);
        case 3:
            return base + (seq_t)16 - s;
        default:
            return (seq_t)0; /* unreachable */
    }
}
