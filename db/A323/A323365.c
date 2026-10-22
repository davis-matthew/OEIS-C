/*
 * A323365  Sum of Stern's Diatomic sequence, A002487 and its Dirichlet
 *          inverse, A317843.
 * https://oeis.org/A323365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = A002487(n) + A317843(n) where A002487 is Stern's diatomic
 *            sequence and A317843 is its Dirichlet inverse. The function
 *            memoises both sequences
 * Range:     i8 n <= 472, i16 n <= 9651, i32 n <= 9651, i64 n <= 9651,
 *            i128 n <= 9651, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Static caches for Stern's sequence (f) and its Dirichlet
        inverse (g). Indices start at 1.  */
    static seq_t *f = NULL;      /* Stern's diatomic sequence */
    static seq_t *g = NULL;      /* Dirichlet inverse of Stern */
    static size_t cap = 0;       /* allocated capacity of the arrays */
    static size_t last = 0;      /* highest index already filled    */

    if (n < 1)                    /* the offset of the sequence is 1 */
        return 0;

    /* Grow the cached arrays if we need to store up to index n.  */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap : 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        f = (seq_t *)realloc(f, new_cap * sizeof(seq_t));
        g = (seq_t *)realloc(g, new_cap * sizeof(seq_t));
        /* Zero-initialize new slots.  */
        for (size_t i = cap; i < new_cap; ++i) {
            f[i] = 0;
            g[i] = 0;
        }
        cap = new_cap;
    }

    /* Initialise base cases (this happens on the first call).  */
    if (last == 0) {
        f[1] = 1;      /* Stern(1) = 1                                    */
        g[1] = 1;      /* its Dirichlet inverse also 1 for n = 1           */
        last   = 1;
    }

    /* Compute all missing values up to n.  */
    for (size_t k = last + 1; k <= (size_t)n; ++k) {
        /* ---- Stern's diatomic sequence ---- */
        if ((k & 1u) == 0u) {                 /* even index */
            f[k] = f[k >> 1];
        } else {                              /* odd index: 2m+1 */
            size_t m = k >> 1;
            f[k] = f[m] + f[m + 1];
        }

        /* ---- Dirichlet inverse (A317843) ---- */
        if (k == 1) {                         /* already set above */
            g[1] = 1;
            continue;
        }

        seq_t sum = 0;
        /* Enumerate all proper divisors d of k, 1 < d < k.  */
        for (idx_t d = 2; d * d <= k; ++d) {
            if (k % d == 0) {
                idx_t d1 = d;
                idx_t d2 = k / d;

                if (d1 > 1u && d1 < k)
                    sum += f[d1] * g[k / d1];

                if (d2 != d1 && d2 > 1u && d2 < k)
                    sum += f[d2] * g[k / d2];
            }
        }

        /* The relation D(k) = -S(k) - sum_{1<d<k} S(d) * D(k/d) .  */
        g[k] = -f[k] - sum;
    }

    last = (size_t)n;
    return f[n] + g[n];
}
