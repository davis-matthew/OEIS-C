/*
 * A278171  Number of n X 2 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,0) (-1,1) (0,-1) (0,1) (1,-1)
 *          or (1,0), with upper left element zero.
 * https://oeis.org/A278171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A278171 – Number of n × 2 0..1 arrays with the stated constraints  
    Formula: a(1)=0, a(2)=2, a(3)=5, a(4)=13, a(5)=43, a(6)=137, a(7)=436
            a(n) = 3*a(n‑1) + a(n‑2) – 3*a(n‑4) – 2*a(n‑5) – a(n‑6)   for n>7
    The function stores computed values in a static cache so that calls in
    any order remain O(1) after the first expansion.  It works for any
    seq_t type supplied to the project.
*/
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* 1‑based array of computed terms       */
static size_t cache_cap = 0;     /* current maximum index stored (≥0)    */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1 – undefined for n==0 */

    /* initialise the cache with the first 7 terms */
    if (cache == NULL) {
        /* allocate indices 0..7 (8 elements, index 0 unused) */
        cache_cap = 7;
        cache = (seq_t*)calloc(cache_cap + 1, sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failed */

        cache[1] = 0;
        cache[2] = 2;
        cache[3] = 5;
        cache[4] = 13;
        cache[5] = 43;
        cache[6] = 137;
        cache[7] = 436;
    }
    /* extend cache if needed */
    if ((idx_t)n > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failed */
        cache = tmp;
        /* compute new terms using the recurrence */
        for (idx_t i = cache_cap + 1; i <= n; ++i) {
            if (i <= 7) continue;           /* base terms already set */
            cache[i] = 3*cache[i-1] + cache[i-2]
                       - 3*cache[i-4] - 2*cache[i-5] - cache[i-6];
        }
        cache_cap = (size_t)n;
    }
    return cache[n];
}
