/*
 * A334281  Number of n-colorings of the vertices of the 4-dimensional cross
 *          polytope such that no two adjacent vertices have the same color.
 * https://oeis.org/A334281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n-3)*(465 - 392*n + 125*n^2 - 18*n^3 +
 *            n^4)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 19, u64 n <= 259, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the cross‑polytope has fewer colors than vertices,
       so the count is zero.  Using the unsigned types directly
       would underflow; we guard against that. */
    if (n < 4)
        return 0;

    seq_t n_t = (seq_t)n;          /* convert index to the arithmetic type */
    seq_t n1   = n_t - 1;
    seq_t n2   = n_t - 2;
    seq_t n3   = n_t - 3;

    /* Evaluate the degree‑4 polynomial using Horner’s rule:
       n^4 - 18 n^3 + 125 n^2 - 392 n + 465           */
    seq_t p = 1;                  /* leading coefficient */
    p = p * n_t - 18;             /* 1*n - 18   -> n - 18          */
    p = p * n_t + 125;            /* (n-18)*n + 125                */
    p = p * n_t - 392;            /* (n^2 -18n +125)*n -392        */
    p = p * n_t + 465;            /* ... + 465                       */

    /* Multiply the four consecutive descending terms and the polynomial */
    seq_t result = n_t * n1;
    result = result * n2;
    result = result * n3;
    result = result * p;

    return result;
}
