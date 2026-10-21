/*
 * A251052  Number of (n+1)X(5+1) 0..3 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements greater than the absolute difference
 *          of its antidiagonal elements.
 * https://oeis.org/A251052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A251052 * Recurrence (for n > 12): * a(n) = 8*a(n-1) -25*a(n-2)
 *            +35*a(n-3) -7*a(n-4) -49*a(n-5) +77*a(n-6) * -55*a(n-7)
 *            +20*a(n-8) -3*a(n-9) * Initial values: * a(1) = 5449, a(2) =
 *            25847, a(3) = 53306, a(4) = 123396, * a(5) = 248803, a(6) =
 *            521785, a(7) = 1045337, a(8) = 2159272, * a(9) = 4456235...
 * Range:     u8 none, u16 n <= 3, u32 n <= 17, u64 n <= 44, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* storage for computed terms (index 1..n)   */
static size_t cache_size = 0;        /* current allocated size (>= n+1)          */
static idx_t computed = 0;           /* highest index whose value is known       */

/* internal helper: initialise cache if necessary */
static void ensure_cache(idx_t n)
{
    const seq_t init[13] = {
        0,                      /* dummy 0‑th entry */
        5449ULL, 25847ULL, 53306ULL, 123396ULL,
        248803ULL, 521785ULL, 1045337ULL, 2159272ULL,
        4456235ULL, 9472092ULL, 20420073ULL, 44991377ULL
    };

    /* Allocate / grow the cache to accommodate index n */
    if (cache == NULL) {
        cache_size = (size_t)n + 1;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return;                 /* allocation failed – abort silently */
        memset(cache, 0, cache_size * sizeof(seq_t));
    } else if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return;
        cache = tmp;
        memset(cache + cache_size, 0,
               (new_size - cache_size) * sizeof(seq_t));
        cache_size = new_size;
    }

    /* Store the initial 12 terms if not already present */
    for (idx_t i = 1; i <= 12 && i <= n; ++i)
        if (cache[i] == 0)
            cache[i] = init[i];

    /* Ensure computed is at least 12 if the cache holds that many entries */
    if (cache_size > 12 && computed < 12)
        computed = 12;
}

/* Main sequence function – can be called in any order */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* value outside the offset – undefined */
    ensure_cache(n);

    /* Compute missing values up to n, if any */
    idx_t start = (computed > 0) ? computed + 1 : 13;
    if (start < 13) start = 13;        /* skip already known initial terms */

    for (idx_t i = start; i <= n; ++i) {
        /* Positive part of the recurrence */
        seq_t pos = 8*cache[i-1] + 35*cache[i-3] + 77*cache[i-6] + 20*cache[i-8];
        /* Negative part of the recurrence */
        seq_t neg = 25*cache[i-2] + 7*cache[i-4] + 49*cache[i-5]
                  + 55*cache[i-7] + 3*cache[i-9];
        cache[i] = pos - neg;           /* pos >= neg for n > 12 */
    }

    computed = (idx_t) ( (n > computed) ? n : computed );
    return cache[n];
}
