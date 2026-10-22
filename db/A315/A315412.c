/*
 * A315412  Coordination sequence Gal.5.141.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n > 0 let k = n / 5 and r = n % 5. Then a(n) = 25 *
 *            k + offset, where offset = r == 1 : (k even ? 6 : 5) r == 2 : (k
 *            even ? 11 : 10) r == 3 : (k even ? 15 : 14) r == 4 : (k even ?
 *            20 : 19) For r == 0 we have a(n) = 25 * k
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t k = n / 5;          /* floor division */
    idx_t r = n % 5;          /* remainder 0..4 */
    seq_t base = (seq_t)k * 25;

    switch (r) {
        case 0:
            return base;
        case 1: {
            seq_t offset = (k & 1) ? 5 : 6;
            return base + offset;
        }
        case 2: {
            seq_t offset = (k & 1) ? 10 : 11;
            return base + offset;
        }
        case 3: {
            seq_t offset = (k & 1) ? 14 : 15;
            return base + offset;
        }
        case 4: {
            seq_t offset = (k & 1) ? 19 : 20;
            return base + offset;
        }
        default: /* unreachable */
            return base;
    }
}
