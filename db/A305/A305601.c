/*
 * A305601  G.f. A(x) satisfies: [x^k] (1+x)^(n*(n+1)/2) * A(x) = 0 for k =
 *          n*(n-1)/2 + 1 through k = n*(n+1)/2 for n >= 1.
 * https://oeis.org/A305601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 25, i64 n <= 48, i128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A305601
   Definition: Let A(x)=∑ a(n) xⁿ.  For every n≥1 and all
   k with n(n‑1)/2 < k ≤ n(n+1)/2 the coefficient of xᵏ in
   (1+x)^{n(n+1)/2}·A(x) is zero.
   The function below builds a(0)…a(n) by solving these linear equations
   sequentially.  a(0)=1.  The algorithm uses only integer arithmetic
   with the type seq_t (plain integer up to 128‑bit or an arbitrary‑precision
   class).  It runs in O(n²) operations; for n≤10 000 it is well below the
   time limit. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_N 10000      /* maximum n expected in this build */
#define MAX_BIN 20000    /* sufficient for all needed binomial rows */

/* static buffers (allowed by the statement – no new/malloc in C++) */
static seq_t a_buf[MAX_N + 1];
static seq_t bin_buf[MAX_BIN + 1];

seq_t a(idx_t n)
{
    /* handle trivial case */
    if (n == 0) return (seq_t)1;

    /* initialise only up to n – no need to clear the whole buffer */
    for (idx_t i = 0; i <= n; ++i) a_buf[i] = 0;
    a_buf[0] = 1;

    idx_t computed = 0;          /* last index that has been finished  */
    idx_t block = 1;             /* current block (n in the description)   */
    idx_t T_prev = 0;            /* T_{block-1} = (block-1)·block/2         */

    while (computed < n) {
        /* T = block·(block+1)/2 = T_block */
        idx_t T = (block * (block + 1)) / 2;

        /* build binomial coefficients for (1+x)^T */
        bin_buf[0] = 1;
        for (idx_t i = 1; i <= T; ++i) {
            seq_t num = (seq_t)(T - i + 1);
            seq_t den = (seq_t)i;
            bin_buf[i] = bin_buf[i - 1] * num / den;
        }

        /* indices in this block that fall inside [0,n] */
        idx_t startK = T_prev + 1;
        if (startK < 1) startK = 1;
        idx_t endK = (n < T) ? n : T;
        if (startK <= endK) {
            for (idx_t k = startK; k <= endK; ++k) {
                seq_t sum = 0;
                for (idx_t i = 1; i <= k; ++i) {
                    sum += bin_buf[i] * a_buf[k - i];
                }
                a_buf[k] = -sum;
            }
            computed = endK;
        }

        /* advance to next block */
        block++;
        T_prev = T;
    }
    return a_buf[n];
}
