/*
 * A315768  Coordination sequence Gal.6.628.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n + 1 - c(n) where c(n) = 1 (for i = 1) + ⌊(n-6)/12⌋ +
 *            1 (if n ≥ 6, for i ≡ 6 mod 12) + ⌊(n-7)/12⌋ + 1 (if n ≥ 7, for i
 *            ≡ 7 mod 12)
 * Range:     u8 n <= 43, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Outputs a(idx_t n) from OEIS A315768. */
seq_t a(idx_t n)
{
    seq_t count = 0;

    /* Count special indices i ≤ n with i == 1 or i ≡ 6,7 (mod 12). */
    if (n >= 1) {
        count = 1;                           /* i == 1 */
        if (n >= 6) {
            count += (seq_t)((n - 6) / 12) + 1;   /* i ≡ 6 (mod 12) */
        }
        if (n >= 7) {
            count += (seq_t)((n - 7) / 12) + 1;   /* i ≡ 7 (mod 12) */
        }
    }

    seq_t six_n  = (seq_t)n * 6;
    seq_t result = six_n + 1 - count;

    return result;
}
