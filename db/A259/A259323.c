/*
 * A259323  a(n) = 2*A259322(n).
 * https://oeis.org/A259323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n*(-31+196*n^2-336*n^4+192*n^6))/21
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 373, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset starts at 1, but guard against 0 */
    /* Use 128‑bit intermediate arithmetic to avoid overflow for n up to 10 000 */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2 = nn * nn;          /* n^2 */
    unsigned __int128 n4 = n2 * n2;          /* n^4 */
    unsigned __int128 n6 = n4 * n2;          /* n^6 */

    /* Polynomial: 384*n^6 - 672*n^4 + 392*n^2 - 62 */
    unsigned __int128 t = (unsigned __int128)384 * n6
                        - (unsigned __int128)672 * n4
                        + (unsigned __int128)392 * n2
                        - (unsigned __int128)62;

    /* a(n) = n * t / 21 */
    unsigned __int128 res = nn * t / 21;

    return (seq_t)res;
}
