/*
 * A351658  G.f. A(x) satisfies: A(x) = 1 - x * A(x/(1 - x)^2) / (1 - x).
 * https://oeis.org/A351658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{k=0..n-1} binomial(n+k-1,n-k-1) * a(k)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 15, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This code uses a recurrence for the binomial factors:
 *      B_k = binomial(n+k-1, n-k-1) = binomial(n+k-1, 2k)
 *
 *  B_{k+1} = B_k * (n+k) * (n-k-1) / ((2k+1)*(2k+2))
 *
 *  The recurrence keeps the intermediate binomial values
 *  well within the range of seq_t for all indices that fit
 *  in the 64‑bit implementation.
 */

#include <stdlib.h>

/* memoisation storage */
static seq_t *cache = NULL;    /* computed terms */
static idx_t  max_n = 0;       /* highest index computed so far */

/* ensure values a(0)..a(n) are computed */
static void compute_up_to(idx_t n)
{
    if (cache == NULL) {
        /* first time: allocate and initialise a(0) */
        size_t new_cap = n + 1;
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!cache) abort();              /* allocation failure */
        cache[0] = (seq_t)1;
        max_n   = 0;
    } else if (max_n >= n) {
        return;                            /* already computed */
    }

    /* enlarge storage if necessary */
    size_t new_cap = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* compute new terms */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t C   = (seq_t)1;                /* B_0 = 1 */

        for (idx_t k = 0; k < m; ++k) {
            sum += C * cache[k];             /* add C*a(k) */

            if (k < m - 1) {                /* prepare next B_k */
                seq_t num = (seq_t)(m + k) * (seq_t)(m - k - 1);
                seq_t den = (seq_t)(2 * k + 1) * (seq_t)(2 * k + 2);
                C = C * num / den;
            }
        }
        cache[m] = -sum;
    }
    max_n = n;
}

/* public routine */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
