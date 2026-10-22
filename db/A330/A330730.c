/*
 * A330730  Determinant of the adjacency matrix for the n^2 X n^2 sudoku
 *          graph.
 * https://oeis.org/A330730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1-n)^(2n^3-4n^2+2n) * (n^2-2n-1)^(n^2-2n+1) * *
 *            (n^2-n-1)^(2n^2-2n) * (2n^2-2n-1)^(2n-2) * (3n^2-2n-1)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 2, i64 n <= 2, i128 n <= 3,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function implements the closed‑form expression given in the OEIS
 *  entry for A330730.  For n=0 and n=1 the graph is degenerate, so the
 *  values are given explicitly.  For all n>=2 the product of the five
 *  factors is evaluated using the supplied big_pow routine, which works
 *  for arbitrary‑precision seq_t as well as for native integer types.  */

#include <stdbool.h>
#include "../utils/big_pow.h"

/* required type definitions are provided by the build system */
seq_t a(idx_t n)
{
    /* Special cases from the OEIS table */
    if (n == 0)  return (seq_t)1;
    if (n == 1)  return (seq_t)0;

    /* Compute each factor separately and multiply them together */
    seq_t res = (seq_t)1;

    /* 1. (-1-n)^(2n^3-4n^2+2n) */
    seq_t base1 = - ( (seq_t)1 + (seq_t)n );
    seq_t exp1  = (seq_t)2*n*n*n - (seq_t)4*n*n + (seq_t)2*n;
    res        *= big_pow(base1, exp1);

    /* 2. (n^2-2n-1)^(n^2-2n+1) */
    seq_t base2 = (seq_t)n*n - (seq_t)2*n - (seq_t)1;
    seq_t exp2  = (seq_t)n*n - (seq_t)2*n + (seq_t)1;
    res        *= big_pow(base2, exp2);

    /* 3. (n^2-n-1)^(2n^2-2n) */
    seq_t base3 = (seq_t)n*n - (seq_t)n - (seq_t)1;
    seq_t exp3  = (seq_t)2*n*n - (seq_t)2*n;
    res        *= big_pow(base3, exp3);

    /* 4. (2n^2-2n-1)^(2n-2) */
    seq_t base4 = (seq_t)2*n*n - (seq_t)2*n - (seq_t)1;
    seq_t exp4  = (seq_t)2*n - (seq_t)2;
    res        *= big_pow(base4, exp4);

    /* 5. (3n^2-2n-1) */
    seq_t base5 = (seq_t)3*n*n - (seq_t)2*n - (seq_t)1;
    res        *= base5;

    return res;
}
