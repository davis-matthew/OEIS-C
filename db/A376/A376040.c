/*
 * A376040  E.g.f. satisfies A(x) = -log(1 - x / (1 - A(x))^3).
 * https://oeis.org/A376040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (3*n+k-2)!/(3*n-1)! * |Stirling1(n,k)| where
 *            |Stirling1(n,k)| are the unsigned Stirling numbers of the first
 *            kind
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for a(n) values */
    static seq_t *cache = NULL;      /* values a(0)..a(last_n) */
    static size_t cache_len = 0;
    static idx_t last_n = 0;         /* maximum n already computed */

    /* static array for Stirling numbers of the first kind (unsigned) */
    static seq_t *s = NULL;          /* s[k] = |S(m,k)| for current m */
    static size_t s_cap = 0;         /* current capacity of s */

    /* ----- handle trivial case a(0) ----- */
    if (n == 0) {
        if (cache_len == 0) {
            cache_len = 1;
            cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
            if (!cache) exit(1);
            cache[0] = 0;
        }
        return cache[0];
    }

    /* ----- make sure cache can hold up to n ----- */
    if (cache_len <= n) {
        size_t old_cap = cache_len;
        cache_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        if (!cache) exit(1);
        for (size_t i = old_cap; i < cache_len; ++i) cache[i] = 0;
    }

    /* If already computed, return cached value */
    if (last_n >= n) return cache[n];

    /* ----- make sure Stirling array can hold current n+1 ----- */
    if (s_cap <= n) {
        size_t old_cap = s_cap;
        s_cap = (size_t)n + 1;
        s = (seq_t *)realloc(s, s_cap * sizeof(seq_t));
        if (!s) exit(1);
        for (size_t i = old_cap; i < s_cap; ++i) s[i] = 0;
    }

    /* ----- compute rows from last_n+1 up to n ----- */
    for (idx_t m = last_n + 1; m <= n; ++m) {
        if (m == 1) {
            s[1] = 1;                      /* |S(1,1)| = 1 */
        } else {
            seq_t mult = (seq_t)(m - 1);
            for (idx_t k = m; k >= 1; --k) {
                seq_t left  = (k > 1) ? s[k - 1] : 0;
                seq_t right = s[k] * mult;
                s[k] = left + right;      /* in-place update */
            }
        }

        /* compute a(m) = Σ_{k=1..m} s[k] * prod_{i=0}^{k-2} (3*m + i) */
        seq_t prod = 1;
        seq_t sum  = 0;
        for (idx_t k = 1; k <= m; ++k) {
            sum += s[k] * prod;
            if (k < m) prod = prod * (seq_t)(3 * m + k - 1);
        }
        cache[m] = sum;
    }

    last_n = n;
    return cache[n];
}
