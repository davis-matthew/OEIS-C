/*
 * A316026  Coordination sequence Gal.3.36.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A316026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6 + 24*floor((n-1)/5) + f((n-1)%5), with f(0)=0, f(1)=3,
 *            f(2)=9, f(3)=12, f(4)=18 (and a(0)=1)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence for Gal.3.36.3 */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 6;

    /* n >= 2 */
    idx_t m    = n - 1;          /* number of differences from k=2 to k=n */
    idx_t full = m / 5;          /* number of complete 5‑term cycles */
    idx_t rem  = m % 5;          /* remaining terms in the current cycle */

    static const seq_t extras[5] = {0, 3, 9, 12, 18};

    seq_t result = (seq_t)6; /* base from a(1) = 6 */
    result += (seq_t)full * (seq_t)24;
    result += extras[rem];
    return result;
}
