/*
 * A314677  Coordination sequence Gal.3.10.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 4*n (0 <= n <= 5) a(n) = 21 + 26*floor((n-5)/6) +
 *            S[(n-5) mod 6] (n >= 6) where S = [0,5,10,14,18,22]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle first five terms directly */
    if (n <= 5)
        return (seq_t)1 + (seq_t)4 * (seq_t)n;

    /* For n >= 6 use the periodic pattern of differences */
    idx_t m     = n - 5;          /* number of steps after a(5) */
    idx_t cycle = m / 6;          /* full 6‑step cycles */
    idx_t rem   = m % 6;          /* remaining steps in the cycle */

    static const seq_t partial[6] = {0, 5, 10, 14, 18, 22};

    /* Compute a(n) using the closed form */
    return (seq_t)21 + (seq_t)26 * (seq_t)cycle + partial[rem];
}
