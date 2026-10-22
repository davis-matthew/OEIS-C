/*
 * A349066  a(n) = H(2*n, n), where H(n,x) is n-th Hermite polynomial.
 * https://oeis.org/A349066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = H(2*n, n) where H(k,x) is the physicists’ Hermite
 *            polynomial
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* H_0(x)=1, H_1(x)=2x, H_{m+1}(x)=2x H_m(x)-2m H_{m-1}(x) */

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* H_0 = 1 */

    /* x = n (the variable at which the polynomial is evaluated) */
    seq_t x = (seq_t)n;

    /* Initial two Hermite values */
    seq_t h_prev = 1;                     /* H_0 */
    seq_t h_curr = 2 * x;                 /* H_1 */

    /* Compute up to degree 2*n */
    idx_t target = 2 * n;
    for (idx_t k = 1; k < target; ++k) {
        /* H_{k+1} = 2x * H_k - 2k * H_{k-1} */
        seq_t term1 = 2 * x * h_curr;
        seq_t term2 = 2 * (seq_t)k * h_prev;
        seq_t h_next = term1 - term2;
        h_prev = h_curr;
        h_curr = h_next;
    }

    return h_curr;   /* H_{2n}(n) */
}
