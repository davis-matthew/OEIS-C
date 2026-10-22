/*
 * A392441  a(n) is the sum of n consecutive triangular numbers immediately
 *          preceding a triangular number T(k), which is also equal to the sum
 *          of n-1 consecutive triangular numbers immediately following T(k).
 * https://oeis.org/A392441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n - 1)*(12*n^3 + 6*n^2 - 5*n - 2) / 6
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 73, u64 n <= 6208,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for correct arithmetic */
    seq_t nn = (seq_t)n;
    seq_t nn_minus = nn - 1;

    /* Compute the factors n*(n-1)  and the polynomial part */
    seq_t factor = nn * nn_minus;
    seq_t n2     = nn * nn;
    seq_t n3     = n2 * nn;
    seq_t t      = (seq_t)12 * n3 + (seq_t)6 * n2 - (seq_t)5 * nn - 2;

    /* Divide by 2 and 3 safely before the heavy multiplication.
       The product n*(n-1) is always even, but it may not contain a factor 3.
       If 3 is not in factor, it must be in t. */
    seq_t d = 6;  /* remaining divisor */

    /* Divide by 2 */
    if ((factor & 1) == 0) {
        factor /= 2;
        d /= 2;
    } else {
        /* This case never happens because n*(n-1) is always even. */
    }

    /* Divide by 3 */
    if ((factor % 3) == 0) {
        factor /= 3;
        d /= 3;
    } else {
        /* The remaining factor 3 must divide t. */
        t /= 3;
        d /= 3;
    }
    /* At this point d should be 1. */

    /* Final multiplication.  For all n where the result fits in seq_t
       the intermediate product fits in seq_t as well because we divided
       out the small factors before the multiplication. */
    return factor * t;
}
