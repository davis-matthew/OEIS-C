/*
 * A335312  A(n, k) = k! [x^k] exp(2*x)*(y*sinh(x*y) + cosh(x*y)) and y =
 *          sqrt(n). Square array read by ascending antidiagonals, for n >= 0
 *          and k >= 0.
 * https://oeis.org/A335312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array A(n,k) =
 *            4*A(n,k-1)+(n-4)*A(n,k-2), A(n,0)=1, A(n,1)=n+2 along ascending
 *            antidiagonals: a(0)=A(0,0), a(1)=A(1,0), a(2)=A(0,1), ... The
 *            implementation computes the appropriate (n,k) and evaluates
 *            A(n,k) by the recurrence, ensuring correct signed arithme...
 * Range:     u8 n <= 31, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find diagonal t such that t*(t+1)/2 <= n < (t+1)*(t+2)/2 */
    idx_t t = 0;
    while ((t + 1) * (t + 2) / 2 <= n) {
        ++t;
    }
    idx_t offset = n - t * (t + 1) / 2;
    idx_t i = t - offset;  /* row index */
    idx_t j = offset;      /* column index */

    if (j == 0) {
        return (seq_t)1;
    }
    if (j == 1) {
        return (seq_t)i + 2;
    }

    seq_t a0 = (seq_t)1;            /* A(i,0) */
    seq_t a1 = (seq_t)i + 2;        /* A(i,1) */
    seq_t a2 = 0;

    for (idx_t k = 2; k <= j; ++k) {
        if (i >= 4) {
            a2 = a1 * ((seq_t)4) + a0 * ((seq_t)(i - 4));
        } else {
            a2 = a1 * ((seq_t)4) - a0 * ((seq_t)(4 - i));
        }
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
