/*
 * A313246  Coordination sequence Gal.6.264.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n ≥ 1: a(n) = a(n‑1) + d(n) The step size d(n) is
 *            defined piecewise: d(1) = 3 for n ≥ 2: if n mod 6 == 0 or 1 →
 *            d(n) = 4 if n mod 6 == 2 or 5 → d(n) = 5 if n mod 6 == 3 let k =
 *            n / 6 (integer division) d(n) = 6 if k is even, else 5 if n mod
 *            6 == 4 let k = n / 6 d(n) = 5 if k is even...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t d;
        if (i == 1) {
            d = 3;
        } else {
            idx_t r = i % 6;
            if (r == 0 || r == 1) {
                d = 4;
            } else if (r == 2 || r == 5) {
                d = 5;
            } else { /* r == 3 or 4 */
                idx_t k = i / 6;   /* integer division */
                if (r == 3) {
                    d = (k & 1) ? 5 : 6;
                } else { /* r == 4 */
                    d = (k & 1) ? 6 : 5;
                }
            }
        }
        val += d;
    }
    return val;
}
