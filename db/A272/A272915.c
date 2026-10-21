/*
 * A272915  a(n) = n + floor(5*n/6).
 * https://oeis.org/A272915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(5*n/6)
 * Range:     u8 n <= 139, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before arithmetic to avoid 64‑bit overflow.
       All intermediate values fit in seq_t on the provided builds. */
    return (seq_t)n + ((seq_t)n * 5 / 6);
}
