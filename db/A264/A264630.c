/*
 * A264630  Number of (3+1)X(n+1) arrays of permutations of 0..n*4+3 with each
 *          element having directed index change -2,0 -1,0 0,-1 or 1,1.
 * https://oeis.org/A264630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A264630 – number of (3+1)×(n+1) arrays of permutations of 0..4·n+3
 *   with directed index changes (-2,0), (-1,0), (0,-1) or (1,1).
 *
 * Recurrence (valid for n > 15):
 *   a(n) = 6·a(n‑2) +10·a(n‑3) + 4·a(n‑4)
 *         - 2·a(n‑5) – a(n‑6) + 4·a(n‑7)
 *         +18·a(n‑8) +25·a(n‑9) – 8·a(n‑10)
 *         – 6·a(n‑11) –10·a(n‑12) + a(n‑15)
 *
 * Initial values (n = 1 … 15) are required to start the recurrence.
 * They are included as a small seed table (not a lookup of the full b‑file).
 */

#include <stdlib.h>

/* Initial values a(1)..a(15) */
static const seq_t init_vals[16] = {
    0ULL,                      /* dummy for 0‑based index */
    2ULL, 4ULL, 13ULL, 40ULL, 125ULL, 393ULL,
    1200ULL, 3759ULL, 11572ULL, 36004ULL,
    111380ULL, 345489ULL, 1070564ULL,
    3318409ULL, 10285305ULL
};

static seq_t *cache      = NULL;   /* first element unused */
static idx_t  cached_up  = 0;
static size_t cap        = 0;

/* ensure that cache can answer all a(k) for k <= n, computing missing ones */
static void ensure_cache(idx_t n)
{
    if (n <= cached_up)
        return;

    /* Expand/allocate cache if necessary */
    if (cache == NULL) {
        /* start with space for first 16 elements (index 0 unused) */
        cap = 16;
        while (cap < n)
            cap <<= 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * (cap + 1));
        if (cache == NULL)    /* allocation failed – let caller deal with it */
            return;
        /* initialise first 15 values (index 0 stays undefined) */
        idx_t start = 1;
        idx_t end   = (n < 15) ? n : 15;
        for (idx_t i = start; i <= end; ++i)
            cache[i] = init_vals[i];
        cached_up = end;
    } else if (cap < n) {
        size_t new_cap = cap;
        while (new_cap < n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        if (tmp == NULL) /* allocation failed – keep old cache */
            return;
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cached_up + 1; i <= n; ++i) {
        if (i <= 15) {          /* just copy the seed value */
            cache[i] = init_vals[i];
            continue;
        }
        seq_t val = 0;
        val += (seq_t)6  * cache[i-2];
        val += (seq_t)10 * cache[i-3];
        val += (seq_t)4  * cache[i-4];
        val -= (seq_t)2  * cache[i-5];
        val -=          cache[i-6];
        val += (seq_t)4  * cache[i-7];
        val += (seq_t)18 * cache[i-8];
        val += (seq_t)25 * cache[i-9];
        val -= (seq_t)8  * cache[i-10];
        val -= (seq_t)6  * cache[i-11];
        val -= (seq_t)10 * cache[i-12];
        val +=          cache[i-15];
        cache[i] = val;
    }
    cached_up = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* behavior for n<1 not specified */
    ensure_cache(n);
    return cache[n];
}
