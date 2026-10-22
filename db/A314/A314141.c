/*
 * A314141  Coordination sequence Gal.5.290.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(16*n/3) + 1 if n>0 and n%9 in {2,4,8}; a(0) = 1
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute floor(16*n/3) safely in 64‑bit arithmetic. */
    unsigned long long tmp = 16ULL * (unsigned long long)n;
    seq_t base = (seq_t)(tmp / 3ULL);

    idx_t rem = n % 9;
    if (rem == 2 || rem == 4 || rem == 8)
        base += 1;

    return base;
}
