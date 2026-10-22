/*
 * A314027  Coordination sequence Gal.5.87.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + (n == 0 ? 1 : (n % 10 == 2 ? 1 : (n % 10 == 8 ? -1
 *            : 0)))
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 5 * n in the target type */
    seq_t base = (seq_t)n * (seq_t)5;

    /* Handle the special case n == 0 */
    if (n == 0)
        return base + 1;

    /* Adjust by +1 or -1 for the periodic pattern */
    switch (n % 10) {
        case 2:          /* n ≡ 2 (mod 10) → +1 */
            return base + 1;
        case 8:          /* n ≡ 8 (mod 10) → -1 */
            return base - 1;
        default:
            return base; /* otherwise the value is exactly 5*n */
    }
}
