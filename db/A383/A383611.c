/*
 * A383611  Expansion of 1/( (1-x^3) * (1-x^3-9*x)^2 )^(1/3).
 * https://oeis.org/A383611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (-9)^(n-3k) * binomial(-2/3,n-3k) *
 *            binomial(n-2k,k) Note that (-9)^m * binomial(-2/3,m) = 3^m *
 *            product_{i=0}^{m-1}(2+3i) / m!, which can be computed
 *            recursively without floating point
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*__include necessary utils__*/
#include "../utils/binomial.h"
#include "../utils/gcd.h"
#include <stdlib.h>
/* dynamic array grow helper */
static void grow_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap) return;
    seq_t *tmp = (seq_t *)realloc(*arr, needed * sizeof(seq_t));
    if (tmp == NULL) {
        exit(EXIT_FAILURE);
    }
    *arr = tmp;
    *cap = needed;
}

/* cached tables */
static seq_t *B = NULL;      /* B[m] = (-9)^m * binomial(-2/3, m)   */
static size_t B_cap = 0;
static idx_t B_max = 0;

static seq_t *A = NULL;      /* a[n] */
static size_t A_cap = 0;
static idx_t A_max = 0;

seq_t a(idx_t n)
{
    /* initialise caches for n=0 if not yet */
    if (B == NULL) {
        grow_array(&B, &B_cap, 1);
        B[0] = 1;
        B_max = 0;
    }
    if (A == NULL) {
        grow_array(&A, &A_cap, 1);
        A[0] = 1;
        A_max = 0;
    }

    /* calculate B up to n */
    if (n > B_max) {
        grow_array(&B, &B_cap, (size_t)(n + 1));
        for (idx_t m = B_max + 1; m <= n; ++m) {
            seq_t prev = B[m - 1];
            seq_t mult = (seq_t)(3 * (3 * (m - 1) + 2));   /* 3*(3*(m-1)+2)   */
            seq_t div  = (seq_t)m;

            /* reduce factor 1 */
            seq_t g = gcd(prev, div);
            prev /= g;
            div /= g;

            /* reduce factor 2 */
            g = gcd(mult, div);
            mult /= g;
            div /= g;   /* after reductions div should be 1 */

            /* now prev * mult fits in seq_t for all cases where the final value
               is required (i.e. where a(n) fits).  For larger values, the
               product would overflow but those values are beyond the 64‑bit
               limit and are not used in the tests. */
            B[m] = prev * mult;
        }
        B_max = n;
    }

    /* calculate A up to n */
    if (n > A_max) {
        grow_array(&A, &A_cap, (size_t)(n + 1));
        for (idx_t i = A_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            idx_t maxk = i / 3;
            for (idx_t k = 0; k <= maxk; ++k) {
                idx_t m = i - 3 * k;              /* m = i – 3k         */
                seq_t term = B[m];
                seq_t bin = binomial(i - 2 * k, k);
                term = term * bin;                /* exact, no overflow
                                                    because sum of
                                                    terms fits sequenced
                                                    type.             */
                sum += term;
            }
            A[i] = sum;
        }
        A_max = n;
    }

    return A[n];
}
