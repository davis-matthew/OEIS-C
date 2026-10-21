/*
 * A245831  The Szeged index of the coronene/circumcoronene benzenoid H_k (see
 *          Fig. 5 of the Gutman & Klavzar reference or Fig. 5.7 of the Diudea
 *          et al. reference).
 * https://oeis.org/A245831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * (36 n^6 - n^4 + n^2) / 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k  = (seq_t)n;          /* n */
    seq_t k2 = k * k;             /* n^2 */
    seq_t k4 = k2 * k2;           /* n^4 */
    seq_t k6 = k4 * k2;           /* n^6 */

    seq_t term1 = k6 * 36;        /* 36 n^6 */
    seq_t term2 = k4;             /* n^4 */
    seq_t term3 = k2;             /* n^2 */

    seq_t expr   = term1 - term2 + term3;   /* 36 n^6 - n^4 + n^2 */
    seq_t result = expr * 3 / 2;            /* multiply by 3, then divide by 2 */

    return result;
}
