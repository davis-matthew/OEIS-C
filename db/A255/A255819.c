/*
 * A255819  Expansion of e.g.f. exp(Sum_{k>=1} k^3 * x^k).
 * https://oeis.org/A255819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n≥1 a(n)=∑_{k=0}^{n-1} (n-k)^4 · ( (n‑1)! / k! ) ·
 *            a(k) This is equivalent to the OEIS definition a(n)=y(n)·n! with
 *            y(0)=1 and y(n)=(1/n)∑_{k=0}^{n‑1}(n‑k)^4y(k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   ensure
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: dynamic cache for a(n); uses a single
   recurrence that avoids fractional intermediate values. */

#include <stdlib.h>
#include <string.h>
#include "../utils/ensure.h"

static seq_t *A      = NULL;   /* cache of a(n)   */
static size_t capA   = 0;      /* current capacity of A   */
static idx_t  maxN   = 0;      /* largest n computed in A   */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* make sure the cache can hold up to n */
    ensure(&capA, &A, n);

    /* initialise base values if not yet done */
    if (maxN == 0) {
        A[0] = (seq_t)1;
        maxN = 0;
    }

    /* compute all missing values up to n */
    for (idx_t m = maxN + 1; m <= n; ++m) {
        seq_t sum  = 0;
        seq_t coef = 1;                     /* (m-1)! / (m-1)! = 1        */
        /* iterate k from m‑1 down to 0   */
        for (idx_t k = m - 1; ; --k) {
            seq_t t  = (seq_t)(m - k);      /* n-k */
            seq_t t2 = t * t;               /* (n‑k)² */
            seq_t t4 = t2 * t2;             /* (n‑k)⁴ */
            sum += t4 * coef * A[k];         /* add term */
            if (k == 0) break;
            coef *= (seq_t)k;                /* update coefficient
                                                coef_{k-1} = coef_k * k   */
        }
        A[m] = sum;
    }
    maxN = n;
    return A[n];
}
