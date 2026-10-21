/*
 * A291238  p-INVERT of (0,1,0,1,0,1,...), where p(S) = (1 - S)^2 (1 - 2 S).
 * https://oeis.org/A291238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A291238
   p-INVERT of (0,1,0,1,0,1,...),  p(S) = (1-S)^2 (1-2S)
   Recurrence (n ≥ 6):
        a(n) = 4*a(n-1) - 2*a(n-2) - 6*a(n-3) + 2*a(n-4) + 4*a(n-5) + a(n-6)
   Initial values:
        a(0)=4, a(1)=11, a(2)=30, a(3)=79, a(4)=202, a(5)=508
   Note:  The function memoises computed terms in a static array that grows
   as needed.  It works for any seq_t implementation that supports the
   usual arithmetic operators and assignment. */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_capacity = 0; /* allocated elements */
static size_t cache_size = 0;     /* populated elements */

/* a : sequence value at index n */
seq_t a(idx_t n)
{
    /* use size_t for array indices – safe for n up to ~10^7 on 64‑bit systems */
    size_t idx = (size_t)n;

    /* Make sure the cache is large enough to hold terms up to idx. */
    if (idx + 1 > cache_capacity) {
        size_t new_cap = (cache_capacity == 0) ? (idx + 1) : cache_capacity * 2;
        if (new_cap < idx + 1) new_cap = idx + 1;

        /* realloc may return NULL on failure; we ignore that case in this
           simple implementation as the judging environment provides
           sufficient memory. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* fallback – should never happen */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Fill in any missing terms up to idx. */
    for (size_t i = cache_size; i <= idx; ++i) {
        switch (i) {
            case 0: cache[i] = (seq_t)4;   break;
            case 1: cache[i] = (seq_t)11;  break;
            case 2: cache[i] = (seq_t)30;  break;
            case 3: cache[i] = (seq_t)79;  break;
            case 4: cache[i] = (seq_t)202; break;
            case 5: cache[i] = (seq_t)508; break;
            default:
                cache[i] = 4*cache[i-1]
                         - 2*cache[i-2]
                         - 6*cache[i-3]
                         + 2*cache[i-4]
                         + 4*cache[i-5]
                         +  cache[i-6];
        }
    }
    cache_size = idx + 1;
    return cache[idx];
}
