/*
 * A317194  Length of smallest cyclic covering word for covering (n-2)-subsets
 *          of an n-set.
 * https://oeis.org/A317194
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 3 if n = 3 a(n) = 8 if n = 4 a(n) = 13 if n = 5 a(n) =
 *            n*(n+1)/2 if n >= 6
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)8;
    if (n == 5) return (seq_t)13;
    if (n >= 6) {
        seq_t nn = (seq_t)n;
        return nn * (nn + (seq_t)1) / (seq_t)2;
    }
    /* Undefined for n < 3 in OEIS sequence, return 0 */
    return (seq_t)0;
}
