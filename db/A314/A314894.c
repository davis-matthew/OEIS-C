/*
 * A314894  Coordination sequence Gal.6.194.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n - c(n) + 1, where c(n) counts the indices m ≤ n with
 *            a(m)-a(m-1)=4. Those indices are 1 and, for k≥0, 2 + 11k, 2 +
 *            11k + 4, 2 + 11k + 8
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0u) {
        return (seq_t)1;
    }

    /* count of indices where the increment is 4 */
    seq_t cnt = 1;               /* the occurrence at n = 1 */

    if (n >= 2u) {
        /* work with the block pattern starting at 2 */
        idx_t rem   = n - 2u;                /* distance from the first block start */
        idx_t bv    = rem / 11u;             /* number of full 11‑block periods */
        idx_t offs  = rem % 11u;             /* position inside the current block 0…10 */

        cnt += bv * 3u;                       /* each full period contributes 3 such indices */
        /* indices inside the current block are at offsets 0, 4, 8 */
        cnt += 1u;                            /* offset 0 is always present for n ≥ 2 */
        if (offs >= 4u) { cnt += 1u; }
        if (offs >= 8u) { cnt += 1u; }
    }

    /* a(n) = 5n - cnt + 1 */
    seq_t result = (seq_t)5 * (seq_t)n - cnt + 1;
    return result;
}
