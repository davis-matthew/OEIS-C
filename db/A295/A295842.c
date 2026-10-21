/*
 * A295842  Number of n X 3 0..1 arrays with each 1 adjacent to 1 or 2
 *          king-move neighboring 1s.
 * https://oeis.org/A295842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A295842: Number of n×3 0..1 arrays with each 1 adjacent to 1 or 2 king‑move neighboring 1s
   Recurrence (empirical):
        a(n) = 4*a(n-1)+a(n-2)+12*a(n-3)-30*a(n-4)+3*a(n-5)-6*a(n-6)+6*a(n-7)+5*a(n-8)+2*a(n-9)
   Initial values:
        a(1)=4, a(2)=29, a(3)=104, a(4)=467, a(5)=2197,
        a(6)=9645, a(7)=43335, a(8)=195508, a(9)=876170
   The function is memoised and can be called in any order. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1‑based memoisation cache */
    static seq_t *cache = NULL;           /* cache[0] is always 0 */
    static idx_t cur_len = 0;             /* highest index currently stored */
    static size_t cache_cap = 0;          /* allocated capacity (size_t) */

    if (n == 0) return 0;                 /* offset starts at 1; 0 is unused */

    /* Ensure the cache can hold indices up to n (inclusive) */
    idx_t needed = n + 1;                 /* number of elements needed */
    if ((size_t)needed > cache_cap) {
        size_t new_cap = (size_t)needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – undefined result */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[0] = 0;                         /* guarantee a(0)=0 for recurrence */

    /* Initial term values (indices 1..9) */
    static const seq_t base_vals[10] = {
        0,          /* dummy for index 0 */
        4, 29, 104, 467, 2197, 9645, 43335, 195508, 876170
    };

    if (cur_len < 9) {
        idx_t start = cur_len + 1;
        if (start < 1) start = 1;
        idx_t end = 9;
        if (end > n) end = n;
        for (idx_t i = start; i <= end; ++i) {
            cache[i] = base_vals[i];
        }
        if (end > cur_len) cur_len = end;
    }

    if (n <= cur_len) return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cur_len + 1; i <= n; ++i) {
        seq_t x = 0;
        x += (seq_t)4  * cache[i-1];
        x += (seq_t)1  * cache[i-2];
        x += (seq_t)12 * cache[i-3];
        x -= (seq_t)30 * cache[i-4];
        x += (seq_t)3  * cache[i-5];
        x -= (seq_t)6  * cache[i-6];
        x += (seq_t)6  * cache[i-7];
        x += (seq_t)5  * cache[i-8];
        x += (seq_t)2  * cache[i-9];
        cache[i] = x;
    }
    cur_len = n;
    return cache[n];
}
