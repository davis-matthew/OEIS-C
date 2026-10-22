/*
 * A343138  Array A(k, n) read by descending antidiagonals: A(k, n) =
 *          Sum_{m=0..n} F(k, m)^2, where F are the k-generalized Fibonacci
 *          numbers A092921.
 * https://oeis.org/A343138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   antidiagonal_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A343138:  a(n) = A(k,n) where (k,n) are the coordinates on the
 * descending antidiagonal of the array A(k,n) = Σ_{m=0..n} F(k,m)^2.
 *
 * F(k,0)=0, F(k,1)=1, and for m≥2,
 *     F(k,m)=Σ_{j=1..k} F(k,m-j)  (omit terms with negative index).
 *
 * For k=0 the sequence is 0,1,1,1,1,...
 *
 * The function interprets the OEIS indexing: a(0) is the first element
 * and a(n) is the n‑th element of the flattened array.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/antidiagonal_index.h"

/* Compute Σ_{m=0..n} F(k,m)^2 for given k,n (both ≤ about 200 for n≤10000). */
static seq_t sum_of_squares(idx_t k, idx_t n)
{
    /* Special case k = 0: 0,1,1,1,1,... */
    if (k == 0) {
        return (n == 0) ? (seq_t)0 : (seq_t)1;
    }

    /* The maximum index needed in the array is 'n'.  For safety we use
     * a small fixed upper bound.  n will never exceed about 200 for n≤10000. */
    seq_t f[256];
    unsigned int kk = (unsigned int)k;
    unsigned int nn = (unsigned int)n;

    f[0] = (seq_t)0;
    f[1] = (seq_t)1;
    for (unsigned int i = 2; i <= nn; ++i) {
        seq_t s = (seq_t)0;
        /* Sum over the previous k terms (or all existing terms if k>i). */
        if (kk <= i) {
            for (unsigned int j = 1; j <= kk; ++j) {
                s += f[i - j];
            }
        } else {
            for (unsigned int j = 1; j < i; ++j) {
                s += f[i - j];
            }
        }
        f[i] = s;
    }

    /* Sum of squares. */
    seq_t res = (seq_t)0;
    for (unsigned int i = 0; i <= nn; ++i) {
        res += f[i] * f[i];
    }
    return res;
}

/* External function as required. */
seq_t a(idx_t n)
{
    /* Determine S such that S*(S+1)/2 ≤ n < (S+1)*(S+2)/2. */
    idx_t S = antidiagonal_index(n);
    idx_t before = S * (S + 1) / 2;
    idx_t d = n - before;              /* offset within the S‑th diagonal */

    /* Coordinates on the descending antidiagonal:
     * (k = d,  n' = S - d). */
    idx_t k = d;
    idx_t new_n = S - d;

    return sum_of_squares(k, new_n);
}
