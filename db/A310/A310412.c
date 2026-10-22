/*
 * A310412  Coordination sequence Gal.5.129.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4. For n ≥ 2, a(n) = a(n−1) + 6 if n mod 9 ∈
 *            {2,4,6,8} a(n) = a(n−1) + 4 otherwise. (These increments
 *            correspond to the recurrence 6,4,6,4,6,4,6,4,4… starting after
 *            a(1).)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;                 /* a(0) */
    if (n == 0) return val;        /* a(0) = 1 */

    val += 3;                      /* a(1) = 4, increment d1 = 3 */

    for (idx_t i = 2; i <= n; ++i) {
        idx_t mod = i % 9;
        if (mod == 2 || mod == 4 || mod == 6 || mod == 8)
            val += 6;
        else
            val += 4;
    }
    return val;
}
