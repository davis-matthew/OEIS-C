/*
 * A314189  Coordination sequence Gal.6.636.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(0) = 1,
 *           a(1) = 5,
 *           for n ≥ 2  a(n) = a(n‑1) + d(n),  
 *           where the difference d(n) is periodic with period 12:
 *               d(n) = { 6, 6, 4, 6, 5, 5, 6, 4, 6, 6, 5, 5 }[ (n‑2) mod 12 ].
 *   Equivalently,
 *     a(n) = 5 + ((n‑1)/12)·64 + Σ_{i=0}^{((n‑1)%12)‑1} d_i ,
 *   with the same period‑12 array d_i above.
 */

static const seq_t inc[12] = { 6, 6, 4, 6, 5, 5, 6, 4, 6, 6, 5, 5 };

seq_t a(idx_t n)
{
    if (n == 0u) return 1u;          /* base case */

    seq_t val = 5u;                  /* a(1) */
    idx_t t     = n - 1u;            /* number of increments to apply */
    idx_t cycles = t / 12u;          /* complete 12‑term cycles */
    idx_t rem    = t % 12u;          /* remaining terms */

    val += (seq_t)64u * (seq_t)cycles;   /* each cycle contributes 64 */

    for (idx_t i = 0u; i < rem; ++i)
        val += inc[i];

    return val;
}
