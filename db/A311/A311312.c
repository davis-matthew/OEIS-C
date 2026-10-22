/*
 * A311312  Coordination sequence Gal.6.118.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n ≥ 1: a(n) = 4*n + C(n) where C(n) is the count
 *            of integers k ≤ n with k ≡ 5,6,7,8 (mod 12). C(n) = ⌊n/12⌋*4 +
 *            max(0, min(n%12-4, 4))
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* full cycles of 12 */
    idx_t full = n / 12;
    idx_t rem  = n % 12;

    /* count of residues 5–8 in the incomplete cycle */
    seq_t added = 0;
    if (rem >= 5) {
        idx_t tmp = rem - 4;              /* rem – 4   (1…8) */
        if (tmp > 4) tmp = 4;             /* cap at 4   */
        added = (seq_t)tmp;               /* cast to seq_t */
    }

    seq_t count5 = (seq_t)full * 4 + added;
    seq_t result = (seq_t)4 * (seq_t)n + count5;

    return result;
}
