/*
 * A348920  a(n) is the "real" part of f(n) = Sum_{k >= 0} g(d_k) * (4 + w)^k
 *          where g(0) = 0 and g(1 + u + 2*v) = (1 + u) * (1 + w)^v for any u
 *          = 0..1 and v = 0..5, Sum_{k >= 0} d_k * 13^k is the base-13
 *          representation of n and w = -1/2 + sqrt(-3)/2 is a primitive cube
 *          root of unity; sequence A348921 gives "...
 * https://oeis.org/A348920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = Re( Sum_{k>=0} g(d_k) * (4 + w)^k ) * where g(0)=0 and
 *            g(1+u+2v)=(1+u)*(1+w)^v for u∈{0,1} and v∈{0,…,5}. * OEIS entry
 *            A348920
 * Range:     i8 n <= 2196, i16 n <= 2196, i32 n <= 2196, i64 n <= 2196,
 *            i128 n <= 2196, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Real and w parts of g(d) for d = 0 … 12 */
    static const seq_t g_real[13] = {0, 1, 2, 1, 2, 0, 0, -1, -2, -1, -2, 0, 0};
    static const seq_t g_w[13]    = {0, 0, 0, 1, 2, 1, 2, 0, 0, -1, -2, -1, -2};

    if (n == 0) return 0;

    seq_t sum_real = 0;
    seq_t powR = 1;   /* real part of (4 + w)^k  */
    seq_t powW = 0;   /* w part   of (4 + w)^k  */

    idx_t tmp = n;
    while (tmp != 0)
    {
        idx_t d = tmp % 13;
        tmp  /= 13;

        /* term = g(d) * (4 + w)^k  ; real part of the product */
        seq_t term = g_real[d] * powR - g_w[d] * powW;
        sum_real += term;

        /* Multiply current power by (4 + w)  (real=4, w=1) */
        seq_t newR = 4 * powR - powW;
        seq_t newW = powR + 3 * powW;

        powR = newR;
        powW = newW;
    }

    return sum_real;
}
