/*
 * A313769  Coordination sequence Gal.6.339.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n > 0: if n mod 12 == 4 -> a(n) = 5*n + 1 if n mod
 *            12 == 8 -> a(n) = 5*n - 1 otherwise -> a(n) = 5*n This
 *            implements OEIS sequence A313769
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)         /* a(0) = 1 */
        return (seq_t)1;

    idx_t r = n % 12;                     /* remainder modulo 12 */
    seq_t val = (seq_t)n * (seq_t)5;      /* base value 5*n */

    if (r == 4)
        val += (seq_t)1;      /* a(n) = 5*n + 1 when n≡4 (mod 12) */
    else if (r == 8)
        val -= (seq_t)1;      /* a(n) = 5*n - 1 when n≡8 (mod 12) */

    return val;
}
