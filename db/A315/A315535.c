/*
 * A315535  Coordination sequence Gal.5.304.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 6*n - [n>=1] - 2*C5 - C2 - C8 where C5 =
 *            floor((n-5)/9)+1 if n>=5, else 0 (indices ≡5 (mod 9)) C2 =
 *            floor((n-2)/9)+1 if n>=2, else 0 (indices ≡2 (mod 9)) C8 =
 *            floor((n-8)/9)+1 if n>=8, else 0 (indices ≡8 (mod 9)) This
 *            expression reproduces the OEIS sequence A315535 for all n≥0
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* counts of indices with residues 5, 2 and 8 modulo 9 */
    idx_t c5 = 0;
    if (n >= 5)
        c5 = (n - 5) / 9 + 1;

    idx_t c2 = 0;
    if (n >= 2)
        c2 = (n - 2) / 9 + 1;

    idx_t c8 = 0;
    if (n >= 8)
        c8 = (n - 8) / 9 + 1;

    seq_t val = 1;
    val += (seq_t)n * 6;               /* 6*n */
    if (n >= 1)
        val -= 1;                      /* subtract overcount for n=1 */
    val -= (seq_t)2 * (seq_t)c5;       /* subtract 2 per residue 5 */
    val -= (seq_t)c2;                  /* subtract 1 per residue 2 */
    val -= (seq_t)c8;                  /* subtract 1 per residue 8 */

    return val;
}
