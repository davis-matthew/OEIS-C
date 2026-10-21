/*
 * A252535  Number of (3+2) X (n+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 2 3 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 37, u64 n <= 85, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = 6*a(n-3) - 8*a(n-6) for n>8
   with a(1)=460, a(2)=334, a(3)=466, a(4)=626,
   a(5)=1120, a(6)=1760, a(7)=2404, a(8)=4304. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of a(1)..a(cache_len) */
static idx_t cache_len = 0;   /* number of elements stored */
static idx_t cache_cap = 0;   /* current allocated capacity */

seq_t a(idx_t n)
{
    /* allocate capacity if this is the first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
    }

    /* ensure capacity for at least n elements */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) { /* OOM, return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill base cases up to 8 if not already present */
    if (cache_len < 8) {
        for (idx_t i = cache_len; i < (n < 8 ? n : (idx_t)8); ++i) {
            switch (i) {
                case 0: cache[i] = 460;  break;
                case 1: cache[i] = 334;  break;
                case 2: cache[i] = 466;  break;
                case 3: cache[i] = 626;  break;
                case 4: cache[i] = 1120; break;
                case 5: cache[i] = 1760; break;
                case 6: cache[i] = 2404; break;
                case 7: cache[i] = 4304; break;
            }
            cache_len++;
        }
    }

    /* if the requested term is already computed, return it */
    if (n <= cache_len) return cache[n - 1];

    /* compute terms from cache_len+1 up to n using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        idx_t idx = i - 1;                     /* zero‑based index */
        seq_t term = 6 * cache[idx - 3] - 8 * cache[idx - 6];
        cache[idx] = term;
        cache_len++;
    }
    return cache[n - 1];
}
