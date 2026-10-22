/*
 * A359130  F(n*(2*n+1))^2 - F(n)^2, where F(t) is the t-th Fibonacci number.
 * https://oeis.org/A359130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(n*(2*n+1))^2 - F(n)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute the small Fibonacci number F(n) */
    seq_t Fn = fib(n);

    /* Compute the large index t = n*(2*n+1) safely as a seq_t first */
    seq_t t_seq = (seq_t)n * ((seq_t)n * 2 + 1);
    idx_t t = (idx_t)t_seq;          /* t fits in idx_t for the required range */

    /* Compute the large Fibonacci number F(t) */
    seq_t Ft = fib(t);

    /* Compute the squares and subtract */
    seq_t sq_t  = Ft * Ft;
    seq_t sq_n  = Fn * Fn;
    return sq_t - sq_n;
}
