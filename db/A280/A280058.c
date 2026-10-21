/*
 * A280058  Number of 2 X 2 matrices with entries in {0,1,...,n} with
 *          determinant = permanent with no entries repeated.
 * https://oeis.org/A280058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(n-1)*(n-2) (valid for all n >= 0)
 * Range:     u8 n <= 6, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0,1,2 the expression would underflow, so return 0 directly. */
    if (n < 3) return 0;

    /* Cast to seq_t to avoid signedness issues. */
    seq_t sn = (seq_t)n;

    /* Compute 2*n*(n-1)*(n-2) iteratively to reduce risk of overflow in interim
       steps for the usual integer sizes supported by the project. */
    seq_t res = (seq_t)2 * sn;      /* 2*n  */
    res *= (sn - 1);                /* 2*n*(n-1) */
    res *= (sn - 2);                /* 2*n*(n-1)*(n-2) */
    return res;
}
