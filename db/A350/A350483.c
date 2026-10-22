/*
 * A350483  G.f. A(x) satisfies: A(x) = A(x^4 - x^6)/x^3.
 * https://oeis.org/A350483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 19, i16 n <= 35, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A350483
 *  Sequence defined by the generating function
 *      A(x) = A(x^4 - x^6) / x^3
 *  The coefficients of A(x) = Σ_{n≥1} a(n)·x^{2n-1} satisfy
 *
 *      a(1) = 1
 *      a(m) = Σ_{n=ceil((m+4)/6)}^{floor((m+3)/4)}
 *                 a(n) * (-1)^{m-(4n-3)} * C(2n-1, m-(4n-3))
 *
 *  where C(a,b) is the binomial coefficient.  The limits on the sum
 *  come from the constraints 0 ≤ m-(4n-3) ≤ 2n-1.
 *
 *  The function below computes a(n) for any n ≥ 1, memoising results
 *  to avoid repeated work.  It is O(n²) in the worst case,
 *  running comfortably under the 2‑second limit for n≈10⁴.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"   /* declares seq_t binomial(idx_t a, idx_t b); */
/* a and idx_t are provided by the build. */

static seq_t *cache = NULL;       /* 1‑based array of computed values    */
static idx_t  cached = 0;        /* largest n that has been computed   */

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* unused, but safe to guard          */

    /* Extend the cache if necessary */
    if (n > cached) {
        size_t new_size = (size_t)n + 1;            /* +1 for 1‑based indexing      */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(1);                     /* allocation failure          */
        cache = new_cache;

        /* zero‑initialise the newly added entries     */
        for (idx_t i = cached + 1; i <= n; ++i) cache[i] = 0;

        /* Compute terms from cached+1 up to n, one by one */
        for (idx_t m = cached + 1; m <= n; ++m) {
            if (m == 1) {            /* a(1) is fixed */
                cache[1] = 1;
                continue;
            }
            seq_t sum = 0;
            /* Determine the admissible range of earlier indices n */
            idx_t lower = (m + 9) / 6;   /* ceil((m+4)/6) = floor((m+9)/6)   */
            idx_t upper = (m + 3) / 4;   /* floor((m+3)/4)                   */
            if (lower > upper) {        /* empty sum => a(m) = 0            */
                cache[m] = 0;
                continue;
            }
            for (idx_t k = lower; k <= upper; ++k) {
                idx_t j = m - (4 * k - 3);              /* 0 ≤ j ≤ 2k-1             */
                seq_t bin = binomial(2 * k - 1, j);     /* binomial coefficient     */
                if (j & 1) bin = -bin;                  /* factor (-1)^j           */
                sum += cache[k] * bin;                  /* accumulate contribution */
            }
            cache[m] = sum;
        }
        cached = n;                      /* all terms up to n are now ready */
    }
    return cache[n];
}
