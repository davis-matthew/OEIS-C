/*
 * A291409  p-INVERT of (1,1,0,0,0,0,...), where p(S) = (1 - S^2)(1 - S)^2.
 * https://oeis.org/A291409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 34, u64 n <= 77, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A291409: p‑INVERT of (1,1,0,0,0,0,…) with p(S)=(1‑S²)(1‑S)²
   Recurrence (n ≥ 9):
      a(n) = 2*a(n‑1) + 2*a(n‑2) – 2*a(n‑3) – 5*a(n‑4) – 2*a(n‑5)
             + 4*a(n‑6) + 4*a(n‑7) + a(n‑8)
   Initial values:
      a(0)=2, a(1)=6, a(2)=14, a(3)=31, a(4)=66,
      a(5)=136, a(6)=272, a(7)=534, a(8)=1030 */
#include <stdlib.h>

/* Static cache for memoisation. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (cache_len <= n) {
        /* Allocate or grow the cache. */
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;      /* Out‑of‑memory: return 0 as fallback. */
        cache = tmp;

        /* Compute values that are not yet cached. */
        idx_t old_len = cache_len;
        cache_len = n + 1;
        for (idx_t i = old_len; i <= n; ++i) {
            switch (i) {
                case 0: cache[i] = 2;   break;
                case 1: cache[i] = 6;   break;
                case 2: cache[i] = 14;  break;
                case 3: cache[i] = 31;  break;
                case 4: cache[i] = 66;  break;
                case 5: cache[i] = 136; break;
                case 6: cache[i] = 272; break;
                case 7: cache[i] = 534; break;
                case 8: cache[i] = 1030;break;
                default:
                    cache[i] = 2 * cache[i - 1] + 2 * cache[i - 2]
                               - 2 * cache[i - 3] - 5 * cache[i - 4]
                               - 2 * cache[i - 5] + 4 * cache[i - 6]
                               + 4 * cache[i - 7] + cache[i - 8];
                    break;
            }
        }
    }
    return cache[n];
}
