/*
 * A309473  a(n) = (-1)^n + Sum_{k=0..n-1} binomial(n-1,k) * a(k) * a(n-k-1).
 * https://oeis.org/A309473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + Sum_{k=0..n-1} binomial(n-1,k) * a(k) * a(n-k-1)
 *            Initial value: a(0) = 1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoised values */
static seq_t *cache      = NULL;      /* computed sequence values          */
static size_t  cap       = 0;         /* allocated capacity                */
static idx_t   last      = (idx_t)-1; /* highest index that has been computed */

/* Recursive definition: a(0)=1, a(n)=(-1)^n + Σ_{k=0}^{n-1} C(n-1,k) a(k) a(n-1-k) */
seq_t a(idx_t n)
{
    /* Ensure enough space for indices 0..n */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise base case on first call */
    if (last == (idx_t)-1) {
        cache[0] = 1;                 /* a(0) = 1 */
        last     = 0;
    }

    /* Compute all missing terms up to n */
    for (idx_t m = last + 1; m <= n; ++m) {
        idx_t len   = m - 1;
        seq_t binom = 1;              /* binomial(len,0)  */
        seq_t sum   = 0;

        for (idx_t k = 0; k <= len; ++k) {
            seq_t prod = cache[k] * cache[len - k];
            prod *= binom;
            sum += prod;

            /* Advance to next binomial coefficient: C(len,k+1) */
            if (k < len) {
                binom = (binom * (seq_t)(len - k)) / (seq_t)(k + 1);
            }
        }

        /* Add the (-1)^m term: +1 if m even, -1 if m odd */
        if ((m & 1) == 0) {
            sum += 1;
        } else {
            sum -= 1;
        }

        cache[m] = sum;
    }

    last = n;
    return cache[n];
}
