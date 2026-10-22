/*
 * A349039  Square array T(n, k) read by antidiagonals, n, k >= 0; T(n, k) =
 *          n^2 - n*k + k^2.
 * https://oeis.org/A349039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (i^2 + j^2) - i*j * where (i, j) is the coordinate read
 *            from the square array * T(i, j) = i^2 - i*j + j^2 by
 *            antidiagonals. * The antidiagonal index `s` and offset `k` are
 *            computed as: * s = largest integer such that s*(s+1)/2 <= n * k
 *            = n - s*(s+1)/2 * i = k * j = s - k * Thus a(n) = T(i, j)
 * Range:     u8 n <= 135, u16 n <= 9836, u32 n <= 9836, u64 n <= 9836,
 *            u128 n <= 9836, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal index `s` such that n lies between
       triangular numbers s*(s+1)/2 and (s+1)*(s+2)/2 - 1. */
    idx_t s = 0, t = 0;
    while (t <= n) {
        ++s;
        t += s;           /* t now holds the (s+1)-th triangular number */
    }
    --s;                  /* step back to the correct diagonal  */
    t = s * (s + 1) / 2;  /* recompute the triangular number for `s` */

    /* Offset within the diagonal */
    idx_t k = n - t;
    idx_t i = k;
    idx_t j = s - k;

    /* Compute T(i, j) = i^2 - i*j + j^2 without underflow:
       (i^2 + j^2) - i*j  */
    seq_t iseq = (seq_t)i;
    seq_t jseq = (seq_t)j;
    seq_t res = iseq * iseq;   /* i^2 */
    res += jseq * jseq;        /* + j^2 */
    res -= iseq * jseq;        /* - i*j */

    return res;
}
