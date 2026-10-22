/*
 * A313927  Coordination sequence Gal.5.135.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 5 a(2) = 10 For n ≥ 3 the increments repeat the
 *            cycle [6,6,6,6,6,5,5,5,5] (length 9). That is, a(n) = a(n‑1) + 6
 *            if (n‑3) mod 9 < 5 a(n) = a(n‑1) + 5 otherwise
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)5;
    if (n == 2u) return (seq_t)10;

    /* n ≥ 3 */
    idx_t m = n - 2u;           /* number of increments from a(2) to a(n) */
    idx_t q = m / 9u;           /* full cycles of 9 increments */
    idx_t r = m % 9u;           /* remaining increments in the last cycle */

    seq_t result = (seq_t)10 + (seq_t)q * (seq_t)50; /* 5*6 + 4*5 = 50 */

    /* add contribution of the incomplete cycle */
    if (r < 5u)
        result += (seq_t)6 * (seq_t)r;
    else
        result += (seq_t)5 * (seq_t)r + (seq_t)5;

    return result;
}
