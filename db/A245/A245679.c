/*
 * A245679  a(n) = pg(n, 3) + pg(n, 4) + ... + pg(n, n) where pg(n, m) is the
 *          m-th n-th-order polygonal number.
 * https://oeis.org/A245679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-6 - n + 2*n^2 - 2*n^3 + n^4)/6 (for n > 1)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 291, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0, a(1) = 0, a(2) = 0 */

/* Computes the nth term of OEIS sequence A245679.  The argument
 * n is of type idx_t (an unsigned 64‑bit integer), and the return
 * value is of type seq_t, which is an unsigned integer type
 * chosen by the build environment.  The implementation uses only
 * integer arithmetic and evaluates the closed‑form polynomial
 * directly, guaranteeing exact results for all n≥0 that fit
 * in seq_t.  For n ≤ 2 the function returns 0 directly. */
seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;

    seq_t N   = (seq_t)n;          /* cast index to the sequence type */
    seq_t N2  = N * N;             /* n^2  */
    seq_t N3  = N2 * N;            /* n^3  */
    seq_t N4  = N3 * N;            /* n^4  */

    /* Evaluate the numerator stepwise to avoid unsigned underflow. */
    seq_t num = N4;
    num -= 2 * N3;                 /* n^4 - 2*n^3 */
    num += 2 * N2;                 /* +2*n^2 */
    num -= N;                      /* -n */
    num -= 6;                      /* -6 */

    return num / 6;                /* integer division truncates . */
}
