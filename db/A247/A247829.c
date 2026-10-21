/*
 * A247829  a(3*k) = k*(k+1), a(3*k+1) = (2*k-1)*(2*k+1), a(3*k+2) =
 *          (2*k-1)*(2*k+3).
 * https://oeis.org/A247829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(3*k) = k*(k+1), a(3*k+1) = (2*k-1)*(2*k+1), a(3*k+2) =
 *            (2*k-1)*(2*k+3), offset 0
 * Range:     i8 n <= 18, i16 n <= 273, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 3U;        /* floor division by 3 */
    idx_t r = n % 3U;        /* remainder 0,1,2 */

    seq_t kk = (seq_t)k;     /* cast index part to seq_t once */
    if (r == 0U) {
        /* a(3*k) = k*(k+1) */
        return kk * (kk + 1);
    }
    seq_t t = 2 * kk - 1;    /* 2*k-1; signed value allowed */
    if (r == 1U) {
        /* a(3*k+1) = (2*k-1)*(2*k+1) */
        return t * (2 * kk + 1);
    }
    /* r == 2U: a(3*k+2) = (2*k-1)*(2*k+3) */
    return t * (2 * kk + 3);
}
