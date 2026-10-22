/*
 * A310371  Coordination sequence Gal.6.258.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n)=4*n + 2*c(n), * where c(n) is the number
 *            of integers k with 2 ≤ k ≤ n such that * k≡2,6,10 (mod 11).
 *            (These k are the indices where the increment * is 6.)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* Count the special indices ≤ n (k≡2,6,10 (mod 11), k≥2). */
    idx_t t     = n - 2;              /* n≥2 ensures non‑negative */
    idx_t blocks = t / 11;            /* full 11‑interval blocks   */
    idx_t rem    = t % 11;            /* remainder inside the block */
    idx_t count = 3 * blocks + 1;     /* 1 for the first special in the block */

    if (rem >= 4) count += 1;          /* 2nd special in the block */
    if (rem >= 8) count += 1;          /* 3rd special in the block */

    seq_t result = (seq_t)n * 4 + (seq_t)count * 2;
    return result;
}
