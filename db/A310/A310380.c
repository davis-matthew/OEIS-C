/*
 * A310380  Coordination sequence Gal.6.322.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, for n ≥ 1 a(n) = 4 + ⌊(n‑1)/11⌋·54 + P[(n‑1) mod 11],
 *            where P[0..10] = {0, 6, 10, 14, 20, 26, 32, 36, 40, 46, 50, 54}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The constant prefix array holds the cumulative sums of the 11‑term period
       of the difference sequence: 6,4,4,6,6,6,4,4,6,4,4. */
    static const idx_t prefix[12] = {
        0ULL, 6ULL, 10ULL, 14ULL, 20ULL, 26ULL, 32ULL, 36ULL,
        40ULL, 46ULL, 50ULL, 54ULL
    };
    const idx_t period_sum = 54ULL;

    if (n == 0U)
        return 1;
    if (n == 1U)
        return 4;

    /* Number of increments from a(1) to a(n) */
    idx_t steps = n - 1U;

    /* How many complete 11‑step periods fit in those steps */
    idx_t full = steps / 11U;
    idx_t rem  = steps % 11U;

    /* Compute a(n) using 64‑bit arithmetic for safety,
       then cast back to the user’s seq_t type. */
    seq_t result = 4;
    result += (seq_t)(full * period_sum);   /* full periods contribution   */
    result += (seq_t)prefix[rem];           /* remaining partial period    */
    return result;
}
