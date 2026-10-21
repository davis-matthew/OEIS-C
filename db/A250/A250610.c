/*
 * A250610  Number of (n+1) X (7+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 128 + 2^(7+n) + 410*n + 219*n^2
 * Range:     u8 none, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A250610: Numbers of  (n+1)×8 0..1 arrays with monotone row/column differences */

/* The sequence has the closed‑form expression a(n)=128+2^(7+n)+410*n+219*n^2
 * provided in the OEIS conjectures.  This implementation evaluates that
 * formula directly using the supplied pow2() routine.  All arithmetic is
 * performed with the build‑specific integer type seq_t. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute each term of the closed form.  Cast intermediate results to
     * seq_t to preserve type consistency and avoid implicit conversion
     * from the generic integer types. */
    seq_t term = (seq_t)128;                     /* constant part       */
    term += pow2(n + 7);                        /* 2^(7+n)             */
    term += (seq_t)410 * n;                     /* linear term         */
    term += (seq_t)219 * n * n;                 /* quadratic term      */
    return term;
}
