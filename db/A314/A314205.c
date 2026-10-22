/*
 * A314205  Coordination sequence Gal.5.133.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1                                 (a(0))
 *            a(n) = 5 + ((n-1)/9)*48 + sum_{k=0}^{(n-1)%9-1} pattern[k]   (n >= 1)
 *   where pattern = {6,6,5,4,5,6,6,5,5}
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t m     = n - 1;                /* number of differences to add */
    idx_t full  = m / 9;                /* full periods of the pattern   */
    idx_t rem   = m % 9;                /* remaining elements of pattern */

    /* pattern of increments after the first special increment (4) */
    static const seq_t pattern[9] = {6, 6, 5, 4, 5, 6, 6, 5, 5};
    static const seq_t prefix[10] = {0, 6, 12, 17, 21, 26, 32, 38, 43, 48};

    seq_t result = (seq_t)5            /* = 1 + first increment 4  */
                  + (seq_t)full * 48
                  + prefix[rem];

    return result;
}
