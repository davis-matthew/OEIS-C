/*
 * A265929  Number of 1 X n 0..4 arrays with the absolute differences of each
 *          element with its with horizontal and antidiagonal neighbors
 *          unique.
 * https://oeis.org/A265929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 5, a(2) = 25, a(3) = 92, a(4) = 340, a(5) = 1252, a(6) =
 *            4616, a(7) = 17012, a(8) = 62696, a(9) = 231044. For n > 9: a(n)
 *            = 3*a(n-1) - 2*a(n-2) + 10*a(n-3) + 27*a(n-4) - 13*a(n-5) +
 *            28*a(n-6) - 20*a(n-7) - 96*a(n-8)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* storage for computed terms */
static idx_t  cache_len = 0;         /* highest index computed (1‑based) */
static size_t cache_cap = 0;         /* allocated capacity */

/* initial 9 terms */
static const seq_t init_terms[9] = {
    (seq_t)5,
    (seq_t)25,
    (seq_t)92,
    (seq_t)340,
    (seq_t)1252,
    (seq_t)4616,
    (seq_t)17012,
    (seq_t)62696,
    (seq_t)231044
};

/* ensure the cache can hold n terms (n is 1‑based) */
static void
ensure_cache_for(idx_t n)
{
    if (cache_len >= n) return;

    /* start with a minimum capacity if not allocated yet */
    if (cache_cap == 0) {
        cache_cap = n;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return; /* allocation failure – caller must handle */
    }
    /* grow if necessary */
    if (n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return; /* allocation failure – caller must handle */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill initial values if they have not yet been set */
    for (idx_t i = cache_len + 1; i <= ((idx_t)9 < n ? (idx_t)9 : n); ++i) {
        cache[i - 1] = init_terms[i - 1];
    }

    /* compute remaining terms using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= ((idx_t)9)) continue; /* already filled */
        /* indices are 1‑based; convert to 0‑based for array access */
        seq_t t1 = cache[i - 2];
        seq_t t2 = cache[i - 3];
        seq_t t3 = cache[i - 4];
        seq_t t4 = cache[i - 5];
        seq_t t5 = cache[i - 6];
        seq_t t6 = cache[i - 7];
        seq_t t7 = cache[i - 8];
        /* recurrence formula */
        cache[i - 1] =
            (seq_t)3 * t1
            - (seq_t)2 * t2
            + (seq_t)10 * t3
            + (seq_t)27 * t4
            - (seq_t)13 * t5
            + (seq_t)28 * t6
            - (seq_t)20 * t7
            - (seq_t)96 * cache[i - 9];
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined but harmless */
    ensure_cache_for(n);
    return cache[n - 1];
}
