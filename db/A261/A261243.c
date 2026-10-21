/*
 * A261243  Row lengths of the irregular triangles A258643 and A261242:
 *          maximal number of 0-islands (holes) of certain bisymmetric n X n
 *          matrices with 0 or 1 entries only.
 * https://oeis.org/A261243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(((n-2)^2)/2) + 1 for n >= 2, a(1) = 1
 * Range:     u8 n <= 17, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)          /* a(1) = 1, a(2) = 1 */
        return (seq_t)1;

    seq_t m = (seq_t)(n - 2);      /* shift origin */
    seq_t mm = m * m;              /* (n-2)^2   */
    seq_t half = (mm + 1) / 2;     /* ceil(mm/2)  */
    return half + 1;               /* final value */
}
