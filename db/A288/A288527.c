/*
 * A288527  Number of (undirected) paths in the grid graph P_3 X P_n.
 * https://oeis.org/A288527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            10*a(n-1)-37*a(n-2)+59*a(n-3)-27*a(n-4)-25*a(n-5)+21*a(n-6)+5*a(n-7)-4*a(n-8)-a(n-9)
 *            for n > 9, with initial values a(1)=3, a(2)=49, a(3)=322,
 *            a(4)=1618, a(5)=7119, a(6)=28917, a(7)=111360, a(8)=412998,
 *            a(9)=1490095
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;    /* 1‑based array, cache[0] unused */
static idx_t  cache_size = 0;   /* highest index currently stored */
static size_t cache_cap = 0;    /* allocated size */

/* Ensure that cache holds values up to index n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    /* enlarge capacity if necessary */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise or extend cache */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        switch (i) {
            case 1:  cache[i] = (seq_t)3;      break;
            case 2:  cache[i] = (seq_t)49;     break;
            case 3:  cache[i] = (seq_t)322;    break;
            case 4:  cache[i] = (seq_t)1618;   break;
            case 5:  cache[i] = (seq_t)7119;   break;
            case 6:  cache[i] = (seq_t)28917;  break;
            case 7:  cache[i] = (seq_t)111360; break;
            case 8:  cache[i] = (seq_t)412998; break;
            case 9:  cache[i] = (seq_t)1490095;break;
            default:
                /* recurrence for n >= 10 */
                cache[i] =
                    10 * cache[i-1]
                  - 37 * cache[i-2]
                  + 59 * cache[i-3]
                  - 27 * cache[i-4]
                  - 25 * cache[i-5]
                  + 21 * cache[i-6]
                  + 5  * cache[i-7]
                  - 4  * cache[i-8]
                  -          cache[i-9];
                break;
        }
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* Off‑by‑one is undefined; return 0 for safety */

    ensure_cache(n);
    return cache[n];
}
