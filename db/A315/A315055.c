/*
 * A315055  Coordination sequence Gal.3.43.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n > 0: let k = n / 6, r = n % 6. r = 0 : a(n) =
 *            32*k - 1 r = 1 : a(n) = (n==1 ? 5 : 28*k + 6) r = 2 : a(n) =
 *            32*k + 9 r = 3 : a(n) = (n==3 ? 15 : 28*k + 16) r = 4 : a(n) =
 *            32*k + 20 r = 5 : a(n) = 28*k + 24
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t k = n / 6;
    idx_t r = n % 6;

    switch (r) {
        case 0:
            return (seq_t)32 * (seq_t)k - 1;
        case 1:
            if (n == 1)
                return 5;
            return (seq_t)28 * (seq_t)k + 6;
        case 2:
            return (seq_t)32 * (seq_t)k + 9;
        case 3:
            if (n == 3)
                return 15;
            return (seq_t)28 * (seq_t)k + 16;
        case 4:
            return (seq_t)32 * (seq_t)k + 20;
        case 5:
            return (seq_t)28 * (seq_t)k + 24;
        default:
            /* unreachable but keep compiler happy */
            return 0;
    }
}
