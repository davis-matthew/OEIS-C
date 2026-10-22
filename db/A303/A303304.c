/*
 * A303304  Generalized 25-gonal (or icosipentagonal) numbers: m*(23*m - 21)/2
 *          with m = 0, +1, -1, +2, -2, +3, -3, ...
 * https://oeis.org/A303304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0 For n≥1 let m = (n%2? (n+1)/2 : -(n/2)) a(n) = m*(23*m -
 *            21)/2 Equivalent (using only non‑negative arithmetic): if n is
 *            odd: a(n) = ((n+1)/2) * (23*((n+1)/2) - 21) / 2 if n is even:
 *            a(n) = ((n+1)/2) * (23*((n+1)/2) + 21) / 2
 * Range:     u8 n <= 9, u16 n <= 150, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return 0;
    /* magnitude of m */
    seq_t mag = (seq_t)((n + 1u) >> 1);   /* (n+1)/2 */
    if (n & 1u) {                         /* odd n → m > 0  */
        return (mag * ((seq_t)23 * mag - (seq_t)21)) / (seq_t)2;
    } else {                              /* even n → m < 0  */
        return (mag * ((seq_t)23 * mag + (seq_t)21)) / (seq_t)2;
    }
}
