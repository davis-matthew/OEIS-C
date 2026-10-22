/*
 * A312897  Coordination sequence Gal.6.192.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   for n = 0 a(0) = 1 for n ≥ 1 a(n) = 4 + ((n-1)/11)*48 +
 *            prefix[(n-1)%11] where prefix =
 *            [0,5,9,13,17,23,27,31,35,40,44,48] and the jumps are
 *            5,4,4,4,6,4,4,4,5,4,4 repeating every 11 steps
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Term for n == 0 */
    if (n == 0) return (seq_t)1;

    /* Periodic difference table */
    static const seq_t diff[11] = {5,4,4,4,6,4,4,4,5,4,4};
    /* Prefix sums of diff[0..i-1] */
    static const seq_t pref[12] = {0,5,9,13,17,23,27,31,35,40,44,48};

    /* Compute (n-1) to get number of steps from a(1) */
    idx_t m = n - 1;
    idx_t q = m / 11;          /* complete periods */
    idx_t r = m % 11;          /* remaining steps */

    /* Compute result using 64‑bit arithmetic then cast to seq_t.
       All intermediate values fit in idx_t for n ≤ 10000. */
    idx_t part = q * (idx_t)48 + (idx_t)pref[r];
    return (seq_t)4 + (seq_t)part;
}
