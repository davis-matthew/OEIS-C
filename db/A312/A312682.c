/*
 * A312682  Coordination sequence Gal.5.112.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0)=1, a(1)=4, a(2)=8; for n≥3,
 * a(n)=a(n-1)+6 if n mod 8 ∈ {3,4,5,6},
 *          a(n)=a(n-1)+4 otherwise.
 * This implements the coordination sequence listed in OEIS A312682.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)1;
    if (n == 1ULL) return (seq_t)4;
    if (n == 2ULL) return (seq_t)8;

    seq_t val = (seq_t)8;            /* a(2) */
    for (idx_t i = 3ULL; i <= n; ++i) {
        seq_t inc = ((i & 7ULL) >= 3ULL && (i & 7ULL) <= 6ULL) ? 6 : 4;
        val += inc;
    }
    return val;
}
