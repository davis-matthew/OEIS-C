/*
 * A317853  a(1) = 1; a(n > 1) = Sum_{0 < k < n} (-1)^(n - k - 1) a(n - k)
 *          Sum_{d|k} a(d).
 * https://oeis.org/A317853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = ∑_{0<k<n} (-1)^(n-k-1) a(n-k) * Σ_{d|k} a(d)
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           (OEIS A317853)                                              */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for n<1, return 0 */

    /* static cache of computed terms */
    static seq_t *a_cache = NULL;       /* a(i) values, 1‑based */
    static seq_t *S_cache = NULL;       /* divisor sums Σ_{d|i} a(d) */
    static idx_t maxn = 0;              /* largest n for which a_cache[1..maxn] are valid */

    /* Grow cache if needed, initialising *only* the newly added entries */
    if (n > maxn) {
        size_t newsize = (size_t)n + 1; /* indices 0..n, we ignore 0 */
        seq_t *tmp_a = (seq_t *)realloc(a_cache, newsize * sizeof(seq_t));
        seq_t *tmp_S = (seq_t *)realloc(S_cache, newsize * sizeof(seq_t));
        if (!tmp_a || !tmp_S) return (seq_t)0;  /* allocation failure */

        /* initialise the new entries */
        for (idx_t i = maxn + 1; i <= n; ++i) {
            tmp_a[i] = (seq_t)0;
            tmp_S[i] = (seq_t)0;
        }
        a_cache = tmp_a;
        S_cache = tmp_S;
    }

    /* Return cached value if already computed */
    if (a_cache[n] != (seq_t)0) return a_cache[n];

    /* Compute terms sequentially up to n, starting after the last computed index */
    for (idx_t idx = (maxn > 0 ? maxn + 1 : 1); idx <= n; ++idx) {
        if (idx == 1) {
            a_cache[1] = (seq_t)1;
            S_cache[1] = a_cache[1];   /* divisor sum of 1 is a(1) */
            continue;
        }

        /* recurrence part */
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        for (idx_t k = 1; k < idx; ++k) {
            seq_t prod = a_cache[idx - k] * S_cache[k];
            if ( ((idx - k - 1) & 1) == 0 )
                pos += prod;   /* even exponent of (-1) -> + */
            else
                neg += prod;   /* odd exponent of (-1) -> - */
        }
        a_cache[idx] = pos - neg;   /* (pos ≥ neg) for this sequence */

        /* divisor sum Σ_{d|idx} a(d) */
        idx_t root = (idx_t)sqrt((double)idx);
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d <= root; ++d) {
            if (idx % d == 0) {
                sum += a_cache[d];
                idx_t other = idx / d;
                if (other != d) sum += a_cache[other];
            }
        }
        S_cache[idx] = sum;
    }

    /* All terms up to n are now valid; update maxn */
    maxn = n;
    return a_cache[n];
}
