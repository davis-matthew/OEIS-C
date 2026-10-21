/*
 * A289590  a(n) is the number of permutations of length n that avoid the
 *          pattern 321 and the mesh pattern (12, 179) or the same sequence
 *          for the mesh patterns (12, 185), (12, 314), (12, 410).
 * https://oeis.org/A289590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*x - (x^3 - x^2 + 2*x - 1)*C(x), where C(x) = (1 -
 *            sqrt(1-4*x))/(2*x) is the Catalan generating function. The
 *            coefficients of the power series give a(n). The function below
 *            computes a(n) for any non‑negative n using only integer
 *            arithmetic
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *catalan = NULL;          /* array C[0..max_n] of Catalan numbers */
static idx_t  catalan_size = 0;        /* number of elements currently stored */

/* Ensure that catalan[0..n] are computed and stored.  Uses O(n) time
   relative to the largest n requested so far. */
static void
ensure_catalan(idx_t n)
{
    /* Allocate or grow the array if needed.  No overflow occurs because
       n <= SIZE_MAX, and we allocate n+1 elements. */
    if (n < catalan_size) return;           /* already have enough */

    size_t newcap = (size_t)(n + 1);
    if (catalan == NULL) {
        catalan = (seq_t *)malloc(newcap * sizeof(seq_t));
    } else {
        catalan = (seq_t *)realloc(catalan, newcap * sizeof(seq_t));
    }

    if (catalan == NULL) {
        /* Out of memory; terminate. */
        exit(EXIT_FAILURE);
    }

    /* Compute Catalan numbers up to n, using the recurrence:
       C_k = C_{k-1} * 2*(2*(k-1)+1) / (k+1)  for k >= 1 and C_0 = 1. */
    if (catalan_size == 0) {
        catalan[0] = (seq_t)1;
        catalan_size = 1;
    }

    for (idx_t k = catalan_size; k <= n; ++k) {
        if (k == 0) {
            catalan[0] = (seq_t)1;
            continue;
        }
        seq_t prev = catalan[k - 1];
        seq_t factor = (seq_t)2 * ( (seq_t)(2 * (k - 1) + 1) );
        seq_t num = prev * factor;
        seq_t denom = (seq_t)(k + 1);
        catalan[k] = num / denom;          /* exact division */
    }

    catalan_size = n + 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 0.  The first terms are
       a(0)=1, a(1)=1, a(2)=1. */
    if (n == 0 || n == 1 || n == 2) {
        return (seq_t)1;
    }

    ensure_catalan(n);                    /* guarantee C[n],C[n-1],C[n-2],C[n-3] exist */

    seq_t val = catalan[n];
    val -= catalan[n - 1] * (seq_t)2;      /* subtraction is safe: C[n] >= 2*C[n-1] for n>=3 */
    val += catalan[n - 2];
    val -= catalan[n - 3];

    return val;
}
