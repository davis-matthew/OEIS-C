/*
 * A276389  Numbers n such that n! ends in the same number of 0's in both base
 *          10 and base 16.
 * https://oeis.org/A276389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k ≥ 0 such that v5(k!) = ⌊v2(k!)/4⌋,
 *            where v2 and v5 are the exponents of 2 and 5 in k!
 * Range:     u8 n <= 145, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t current = 0;           /* current integer k */
    seq_t v2 = 0, v5 = 0;        /* cumulative exponents of 2 and 5 in k! */
    seq_t count = 0;             /* how many qualifying numbers found */

    for (;; ++current) {
        if (current > 0) {
            seq_t x = (seq_t)current;
            /* add exponent of 2 for current */
            while ((x & 1ULL) == 0) {
                ++v2;
                x >>= 1;
            }
            /* add exponent of 5 for current */
            while (x % 5 == 0) {
                ++v5;
                x /= 5;
            }
        }

        /* check condition 4*v5 ≤ v2 < 4*v5 + 4  (holds for k = 0 as well) */
        if ((4 * v5 <= v2) && (v2 < 4 * v5 + 4)) {
            ++count;
            if (count == n)
                return (seq_t)current;
        }
    }
}
