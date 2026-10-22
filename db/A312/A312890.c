/*
 * A312890  Coordination sequence Gal.6.115.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0: a(n) = 4*n + (n ≥ 2 ? ((n-2)/12 + 1) : 0) +
 *            (n ≥ 11 ? ((n-11)/12 + 1) : 0)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of sequence A312890.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* first term */

    /* base value 4*n fits in seq_t for all practical n (e.g., n ≤ 10 000) */
    seq_t res = 4 * (seq_t)n;

    if (n >= 2)
        res += ((seq_t)(n - 2) / 12) + 1;  /* count indices ≡ 2 (mod 12) */

    if (n >= 11)
        res += ((seq_t)(n - 11) / 12) + 1; /* count indices ≡ 11 (mod 12) */

    return res;
}
