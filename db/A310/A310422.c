/*
 * A310422  Coordination sequence Gal.6.325.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 4 For n ≥ 2: a(n) = 4 + 52 * floor((n-1)/10) +
 *            S[(n-1) mod 10] where S = {0,6,10,16,22,28,34,38,44,48,52} (S[0]
 *            = 0 corresponds to no extra terms) This reproduces
 *            1,4,10,14,20,26,32,38,42,48,52,56,62,66,72,78,84,90,94,100,104,108,114,118,
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t S[11] = {0,6,10,16,22,28,34,38,44,48,52};

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;

    /* n ≥ 2 */
    idx_t m = n - 1;               /* number of terms from i = 2 to n inclusive */
    idx_t q = m / 10;              /* full 10‑term cycles */
    idx_t r = m % 10;              /* remaining terms in the last cycle */

    seq_t res = 4;                 /* a(1) */
    res += (seq_t)q * 52;          /* 52 per full cycle */
    res += S[r];                   /* partial cycle contribution */

    return res;
}
