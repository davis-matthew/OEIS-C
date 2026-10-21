/*
 * A294609  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of Product_{j>=1} 1/(1-j*x^j)^(j^(k*j))
 *          in powers of x.
 * https://oeis.org/A294609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(0,k)=1; A(n,k) = (1/n) * Sum_{j=1..n} (Sum_{d|j}
 *            d^(k*d+1+j/d)) * A(n-j,k) (n>0) a(n) = A(i,k) where (i,k) are
 *            the row/column of the square array read by antidiagonals (offset
 *            0)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 39, u64 n <= 60, u128 n <= 110,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/big_pow.h"

static seq_t *A_cache = NULL;   /* 2‑D flat array */
static size_t dim = 0;          /* current dimension (rows = cols = dim) */

/* Ensure that A_cache has size at least need×need, recomputing all values. */
static void ensure_size(size_t need)
{
    if (dim >= need) return;

    /* resize */
    seq_t *new_cache = (seq_t *)realloc(A_cache, need * need * sizeof(seq_t));
    if (!new_cache) abort();
    A_cache = new_cache;

    for (size_t n = 0; n < need; ++n)
    {
        for (size_t k = 0; k < need; ++k)
        {
            if (n == 0)
            {
                A_cache[n * need + k] = (seq_t)1;
            }
            else
            {
                seq_t sum = (seq_t)0;
                for (size_t j = 1; j <= n; ++j)
                {
                    seq_t inner = (seq_t)0;
                    for (size_t d = 1; d * d <= j; ++d)
                    {
                        if (j % d == 0)
                        {
                            /* divisor d */
                            seq_t expd = (seq_t)k * (seq_t)d + (seq_t)1 + (seq_t)(j / d);
                            inner += big_pow((seq_t)d, expd);

                            /* paired divisor j/d */
                            if (d != j / d)
                            {
                                seq_t d2 = (seq_t)(j / d);
                                seq_t expd2 = (seq_t)k * d2 + (seq_t)1 + (seq_t)d;
                                inner += big_pow(d2, expd2);
                            }
                        }
                    }
                    seq_t Aqk = A_cache[(n - j) * need + k];
                    sum += inner * Aqk;
                }
                A_cache[n * need + k] = sum / (seq_t)n;   /* exact division */
            }
        }
    }

    dim = need;
}

/* Main function requested by the build system. */
seq_t a(idx_t n)
{
    size_t N = (size_t)n;

    /* Find diagonal d such that d*(d+1)/2 <= N < (d+1)*(d+2)/2   */
    size_t s = 0;
    while ((s * (s + 1)) / 2 <= N) ++s;
    --s;                      /* now s*(s+1)/2 <= N < (s+1)*(s+2)/2 */

    size_t t_s = (s * (s + 1)) / 2;       /* cumulative elements before this diagonal */
    size_t offset = N - t_s;              /* position within the diagonal (0‑based) */

    size_t i = offset;                    /* row index  (0 ≤ i ≤ s)   */
    size_t k = s - offset;                /* column index (0 ≤ k ≤ s) */

    ensure_size(s + 1);
    return A_cache[i * dim + k];
}
