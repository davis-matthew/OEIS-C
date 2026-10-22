/*
 * A338795  Each term of A003215 (centered hexagonal numbers) is multiplied by
 *          the corresponding term of A003154 (centered dodecagonal numbers).
 * https://oeis.org/A338795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*n^4 - 36*n^3 + 27*n^2 - 9*n + 1
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */

    seq_t res = n4 * (seq_t)18;  /* 18*n^4 */
    res -= n3 * (seq_t)36;       /* - 36*n^3 */
    res += n2 * (seq_t)27;       /* + 27*n^2 */
    res -= nn * (seq_t)9;        /* - 9*n */
    res += (seq_t)1;             /* + 1 */

    return res;
}
