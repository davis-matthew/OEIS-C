/*
 * A365603  Expansion of e.g.f. 1 / (1 - 5 * log(1 + x))^(4/5).
 * https://oeis.org/A365603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} (Π_{j=0..k-1} (5*j+4)) · Stirling1(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses Bell‐style dynamic programming with Stirling first–kind numbers
 * and a running product for the factor Π_{j=0..k-1}(5*j+4).
 * The result is stored in a static cache so that arbitrary calls
 * to a(idx_t) are O(n) in the maximal index seen so far. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/ensure_capacity.h"

/* external integer types defined by the build */
extern seq_t a(idx_t n);

/* implementation */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static bool initialized = false;
    static seq_t *cache   = NULL; /* a(0..max)      */
    static seq_t *s_arr   = NULL; /* current Stirling row */
    static seq_t *P_arr   = NULL; /* product Π(5*j+4) */
    static size_t cap_cache = 0, cap_s = 0, cap_P = 0;
    static idx_t cached_max = 0;

    /* one‑time allocation and initialisation */
    if (!initialized) {
        ensure_capacity(1, &cap_cache, (void **)&cache, sizeof(seq_t));
        ensure_capacity(1, &cap_s,   (void **)&s_arr,   sizeof(seq_t));
        ensure_capacity(1, &cap_P,   (void **)&P_arr,   sizeof(seq_t));

        cache[0] = 1;
        s_arr[0] = 1;
        P_arr[0] = 1;
        cached_max = 0;
        initialized = true;
    }

    /* extend the cache up to the requested index if necessary */
    if (n > cached_max) {
        size_t needed = (size_t)n + 1;
        ensure_capacity(needed, &cap_cache, (void **)&cache, sizeof(seq_t));
        ensure_capacity(needed, &cap_s,   (void **)&s_arr,   sizeof(seq_t));
        ensure_capacity(needed, &cap_P,   (void **)&P_arr,   sizeof(seq_t));

        for (idx_t m = cached_max + 1; m <= n; ++m) {
            /* update product Π_{j=0..m-1}(5*j+4) */
            seq_t multiplier = (seq_t)5 * (m - 1) + 4;
            P_arr[m] = P_arr[m - 1] * multiplier;

            /* compute next row of Stirling numbers of the first kind
             * using the recurrence: s(m,k) = s(m-1,k-1) - (m-1)*s(m-1,k)
             * update in reverse k order to keep old values */
            for (idx_t k = m; k >= 1; --k) {
                seq_t old_s_k = s_arr[k];
                seq_t tmp = s_arr[k - 1];
                s_arr[k] = tmp - (seq_t)(m - 1) * old_s_k;
            }
            s_arr[0] = 0;   /* s(m,0) = 0 for m>0 */

            /* compute a(m) = Σ_{k=1..m} P(k) * s(m,k) */
            seq_t sum = 0;
            for (idx_t k = 1; k <= m; ++k) {
                sum += P_arr[k] * s_arr[k];
            }
            cache[m] = sum;
        }
        cached_max = n;
    }

    return cache[n];
}
