/*
 * A250585  Number of (n+1) X (1+1) 0..2 arrays with nondecreasing
 *          max(x(i,j),x(i,j-1)) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) - 12*a(n-3) + 21*a(n-4) + 3*a(n-5) -
 *            15*a(n-6) + 3*a(n-7) + 3*a(n-8) - a(n-9) Initial values:
 *            a(1)=40, a(2)=167, a(3)=639, a(4)=2375, a(5)=8625, a(6)=30952,
 *            a(7)=110187, a(8)=390391, a(9)=1378890 Note: The sequence is
 *            defined for n >= 1. For n < 1 the function returns 0...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* cached a(1) .. a(k) in cache[0..k-1] */
static size_t cache_len = 0;  /* number of elements stored in cache */
static size_t cache_cap = 0;  /* allocated capacity of cache */

/* Grow cache to at least 'need' elements.  */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < need)
        new_cap = new_cap * 2 + 4;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* Allocation failed: abort, as we cannot continue safely. */
        abort();
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n == 0, return 0 */

    /* Convert idx_t to size_t for indexing.  */
    size_t idx = (size_t)n;           /* idx is 1‑based index */

    /* Extend cache if needed.  */
    if (idx > cache_len) {
        ensure_capacity(idx);
        /* Initial terms a(1)..a(9)  */
        static const seq_t init[9] = {
            40ULL,     167ULL,     639ULL,     2375ULL,     8625ULL,
            30952ULL, 110187ULL, 390391ULL, 1378890ULL
        };
        for (size_t i = cache_len; i < idx; ++i) {
            if (i < 9) {
                cache[i] = init[i];
            } else {
                /* recurrence for a(i+1) with i >= 9  */
                seq_t val = 0;
                val += 6 *  cache[i - 1];
                val -= 7 *  cache[i - 2];
                val -= 12 * cache[i - 3];
                val += 21 * cache[i - 4];
                val += 3  * cache[i - 5];
                val -= 15 * cache[i - 6];
                val += 3  * cache[i - 7];
                val += 3  * cache[i - 8];
                val -= 1  * cache[i - 9];
                cache[i] = val;
            }
        }
        cache_len = idx;
    }
    return cache[idx - 1];   /* return a(n)  */
}
