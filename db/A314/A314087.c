/*
 * A314087  Coordination sequence Gal.5.67.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = 48*⌊n/10⌋ + base[n%10] where
 *            base[1..9] = {5, 11, 16, 20, 24, 28, 32, 37, 43}. Since a(10k) =
 *            48k for k ≥ 1, the value for remainder 0 is handled specially
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    const seq_t base[10] = {0, 5, 11, 16, 20, 24, 28, 32, 37, 43};
    idx_t k = n / 10;
    idx_t r = n % 10;

    if (r == 0)
        return (seq_t)48 * (seq_t)k;
    else
        return base[r] + (seq_t)48 * (seq_t)k;
}
