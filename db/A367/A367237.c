/*
 * A367237  G.f. satisfies A(x) = 1 + x*A(x)^2 / (1 - x*A(x)^2)^2.
 * https://oeis.org/A367237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binom(2n+1,k) * binom(n+k-1, n-k) / (2n+1)
 *            (The recurrence below implements the same formula.)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Ensure capacity for the cache array */
static inline void ensure_cache(idx_t n, seq_t **ptr, size_t *cap)
{
    if (*cap > n) return;
    size_t new_cap = (*cap == 0) ? 1 : *cap;
    while (new_cap <= n) new_cap <<= 1;
    *ptr = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Compute a(n) with memoisation */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    if (n < cache_len) return cache[n];

    ensure_cache(n, &cache, &cache_cap);

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        seq_t C = 1;      /* binom(2i+1,0) */
        seq_t D = 0;      /* will be set for k >= 1 */

        for (idx_t k = 1; k <= i; ++k) {
            /* C = C * (2i+1 - (k-1)) / k */
            C = C * (seq_t)(2*i + 1 - (k-1));
            C = C / (seq_t)k;

            /* Update D */
            if (k == 1) {
                D = (seq_t)i;                       /* binom(i, i-1) = i */
            } else {
                seq_t numD = (seq_t)(i + k - 1) * (seq_t)(i - k + 1);
                seq_t denD = (seq_t)(2*k - 1) * (seq_t)(2*k - 2);
                D = D * numD / denD;
            }

            sum += C * D;
        }
        seq_t denom = (seq_t)(2*i + 1);
        cache[i] = sum / denom;
    }

    cache_len = n + 1;
    return cache[n];
}
