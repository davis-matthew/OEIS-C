/*
 * A315397  Coordination sequence Gal.5.55.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1 define the step d(n) by if (n == 1) d(n) = 5
 *            else if (n % 10 == 0 || n % 10 == 1) d(n) = 6 else if (n % 10 ==
 *            2) d(n) = 5 else if (n % 10 == 9) d(n) = 5 else d(n) = 4 Then
 *            a(n) = a(n‑1) + d(n). This yields a(n) = 1 + prefix[n] for n ≤
 *            10, a(n) = 1 + 45 + 46·((n-10)/10) + cycle_...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Prefix sums for the first 10 steps (indices 1..10). */
    static const seq_t pre_first[11] = {
        0,  /* unused for index 0 */
        5, 10, 14, 18, 22, 26, 30, 34, 39, 45
    };

    if (n <= 10u)
        return (seq_t)1 + pre_first[n];

    /* Prefix sums for the repeating 10‑step cycle that starts at index 11. */
    static const seq_t cycle_pre[11] = {
        0, 6, 11, 15, 19, 23, 27, 31, 35, 40, 46
    };

    idx_t full_cycles = (n - (idx_t)10) / (idx_t)10;
    idx_t remainder   = (n - (idx_t)10) % (idx_t)10;

    seq_t sum = (seq_t)45 + (seq_t)46 * (seq_t)full_cycles + cycle_pre[remainder];

    return (seq_t)1 + sum;
}
