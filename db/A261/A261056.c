/*
 * A261056  Expansion of ( 2-x^2 ) / (x^2+2*x-1)^2 .
 * https://oeis.org/A261056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=2, a(1)=8, a(2)=27, a(3)=84,
   a(n) = 4*a(n‑1) – 2*a(n‑2) – 4*a(n‑3) – a(n‑4), for n ≥ 4   */
#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache of computed terms */
static size_t cache_cap = 0;      /* allocated size of cache      */
static idx_t  cache_max = 0;      /* highest index computed       */

/* Ensure the cache contains a(0)…a(n) */
static void ensure_seq(idx_t n)
{
    /* First call – allocate and initialise the first four terms */
    if (cache_cap == 0) {
        cache_cap = n + 1 > 4 ? (n + 1) : 4;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!seq_cache) exit(1);
        seq_cache[0] = 2;
        if (cache_cap > 1) seq_cache[1] = 8;
        if (cache_cap > 2) seq_cache[2] = 27;
        if (cache_cap > 3) seq_cache[3] = 84;
        cache_max = 3;
    }

    /* Grow the cache if necessary */
    if (n > cache_max) {
        size_t needed = n + 1;
        if (needed > cache_cap) {
            cache_cap = needed;
            void *tmp = realloc(seq_cache, cache_cap * sizeof(seq_t));
            if (!tmp) { free(seq_cache); exit(1); }
            seq_cache = (seq_t *)tmp;
        }

        /* Compute the missing terms */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t t1 = seq_cache[i - 1];
            seq_t t2 = seq_cache[i - 2];
            seq_t t3 = seq_cache[i - 3];
            seq_t t4 = seq_cache[i - 4];

            /* val = 4*t1 - 2*t2 - 4*t3 - t4 */
            seq_t val = t1 + t1;          /* 2*t1 */
            val += val;                  /* 4*t1 */
            val -= t2 + t2;              /* -2*t2 */
            seq_t tmp = t3 + t3;
            tmp += tmp;                  /* 4*t3 */
            val -= tmp;                  /* -4*t3 */
            val -= t4;                   /* -t4 */

            seq_cache[i] = val;
        }
        cache_max = n;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_seq(n);
    return seq_cache[n];
}
