/*
 * A261001  Numbers Q_n arising in computation of number of unordered
 *          unilabeled-bilabeled increasing trees.
 * https://oeis.org/A261001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by the recurrence a(0)=0, a(1)=1 and for n≥0,
 *            a(n+2) = Σ_{i=0}^{n} C(n,i) [a(i+1)a(n-i+1) + a(i)a(n-i+1)]
 *            where C(n,i)=n!/(i!(n-i)!) is the binomial coefficient
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence comes from the differential equation
   A''(x) = A'(x)*(A'(x)+A(x)) for the EGF A(x). */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoized values a[0..k] */
static idx_t cache_size = 0;  /* highest computed index */

/* Ensure that a[0..n] are computed */
static void ensure_cache_upto(idx_t n)
{
    if (n <= cache_size) return;       /* already computed */

    /* Resize the array */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();                  /* allocation failed */
    cache = tmp;

    /* initialise base values if not yet set */
    if (cache_size < 1) {
        cache[0] = 0;
        cache[1] = 1;
        cache_size = 1;
    }

    /* compute new terms up to n */
    for (idx_t cur = cache_size + 1; cur <= n; ++cur) {
        idx_t m = cur - 2;              /* index for a[n+2] recurrence */
        seq_t sum = 0;
        seq_t bin = 1;                  /* binom(m,0) */

        for (idx_t i = 0; i <= m; ++i) {
            seq_t prod1 = cache[i + 1] * cache[m - i + 1];
            seq_t prod2 = cache[i]      * cache[m - i + 1];
            sum += bin * (prod1 + prod2);

            /* update binom(m,i+1) */
            if (i < m) {
                bin = bin * (seq_t)(m - i);
                bin = bin / (seq_t)(i + 1);
            }
        }
        cache[cur] = sum;
    }
    cache_size = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    ensure_cache_upto(n);
    return cache[n];
}
