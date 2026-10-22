/*
 * A315916  Coordination sequence Gal.4.128.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(4k) = 20k (k ≥ 1) a(4k+1) = 6(4k+1) a(4k+2) = 8 if k
 *            = 0 (n = 2) a(4k+2) = 16k + 12 if k ≥ 1 a(4k+3) = 6(4k+3) – 4
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    switch (n & 3U) {
        case 0: // n % 4 == 0
            return (seq_t)20 * ((seq_t)(n >> 2));

        case 1: // n % 4 == 1
            return (seq_t)6 * (seq_t)n;

        case 2: // n % 4 == 2
            if (n == 2)  return (seq_t)8;
            {
                idx_t k = (n - 2) >> 2;          // k = (n-2)/4
                return (seq_t)16 * (seq_t)k + (seq_t)12;
            }

        case 3: // n % 4 == 3
            return (seq_t)6 * (seq_t)n - (seq_t)4;

        default: // unreachable
            return (seq_t)0;
    }
}
