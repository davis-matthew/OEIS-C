/*
 * A312555  Coordination sequence Gal.5.110.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n) = 4*n + 2*⌊(n+6)/9⌋ + 2*⌊(n+4)/9⌋ +
 *            2*⌊(n+2)/9⌋ (the sum counts how many integers k≡3,5,7 (mod 9)
 *            lie in [2..n])
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t n_t = (seq_t)n;
    seq_t s = ((seq_t)(n + 6) / 9) + ((seq_t)(n + 4) / 9) + ((seq_t)(n + 2) / 9);

    return 4 * n_t + 2 * s;
}
