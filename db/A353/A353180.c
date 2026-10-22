/*
 * A353180  Expansion of e.g.f. 1/(1 - Sum_{k>=1} x^(k^2) / (k^2)!).
 * https://oeis.org/A353180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(sqrt(n))} binomial(n, k^2) * a(n-k^2);
 *            a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* integer floor square root using binary search */
static inline idx_t int_isqrt(idx_t n) {
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0 || mid <= n / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* binomial coefficient binom_idx(n, k) computed in seq_t */
static inline seq_t binom_idx(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* ensure cache size */
    if (n >= cap) {
        size_t new_cap = (cap == 0) ? (n + 1) : (cap * 2);
        if (new_cap < n + 1) new_cap = n + 1;

        seq_t *tmp = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (!tmp) abort();

        if (cache) {
            for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cap   = new_cap;
    }

    if (n == 0) {          /* base value a(0) = 1 */
        cache[0] = 1;
        return cache[0];
    }

    if (cache[n] != 0) return cache[n];  /* already computed */

    /* dynamic programming up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0) continue;    /* skip already known */

        idx_t lim = int_isqrt(i);
        seq_t sum = 0;
        for (idx_t k = 1; k <= lim; ++k) {
            idx_t ksq = k * k;
            seq_t bc  = binom_idx(i, ksq);
            seq_t prod = bc * cache[i - ksq];
            sum += prod;
        }
        cache[i] = sum;
    }

    return cache[n];
}
