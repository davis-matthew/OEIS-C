/*
 * A314251  Coordination sequence Gal.5.294.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1: a(n) = 6*n - 1 - S(n), where S(n) = #{ k |
 *            2 <= k <= n and k ≡ 0 or 1 (mod 9) }. This reproduces the
 *            pattern of increments 4,6,6,…,5,5,6,…
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }

    /* Base part 6*n - 1 */
    seq_t result = 6 * (seq_t)n - 1;

    /* Count of indices k (2 <= k <= n) with k ≡ 0 (mod 9) */
    idx_t count0 = 0;
    if (n >= 9) {
        count0 = ((n - 9) / 9) + 1;
    }

    /* Count of indices k (2 <= k <= n) with k ≡ 1 (mod 9) */
    idx_t count1 = 0;
    if (n >= 10) {
        count1 = ((n - 10) / 9) + 1;
    }

    /* Number of reduced‐difference indices */
    seq_t S = (seq_t)count0 + (seq_t)count1;

    return result - S;
}
