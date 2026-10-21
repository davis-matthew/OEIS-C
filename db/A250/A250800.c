/*
 * A250800  Number of (3+1) X (n+1) 0..1 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n−1) − 8*a(n−2) + 3*a(n−3) + 3*a(n−4) − 2*a(n−5),
 *            a(1)=58, a(2)=180, a(3)=482, a(4)=1190, a(5)=2776
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* 1‑based cache of computed values   */
static idx_t cached_n = 0;      /* highest index computed            */
static size_t capacity   = 0;   /* current allocated size (1‑based)   */

seq_t a(idx_t n)
{
    /* n must be >= 1; treat n==0 as 0 for safety */
    if (n == 0) return 0;

    /* Allocate initial cache if needed */
    if (cache == NULL) {
        capacity = (size_t)n + 10;
        cache = (seq_t*)malloc((capacity + 1) * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure */

        /* Initial values a(1)..a(5) */
        cache[0] = 0;                 /* unused */
        cache[1] = (seq_t)58;
        cache[2] = (seq_t)180;
        cache[3] = (seq_t)482;
        cache[4] = (seq_t)1190;
        cache[5] = (seq_t)2776;
        cached_n = 5;
    }

    /* Grow cache if necessary */
    if ((size_t)n > capacity) {
        size_t new_cap = (capacity * 3) / 2 + 10;   /* grow by 50% */
        if (new_cap < (size_t)n) new_cap = n + 10;
        seq_t *tmp = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing values using the recurrence */
    if (n > cached_n) {
        for (idx_t k = cached_n + 1; k <= n; ++k) {
            cache[k] =
                5 * cache[k - 1]
                - 8 * cache[k - 2]
                + 3 * cache[k - 3]
                + 3 * cache[k - 4]
                - 2 * cache[k - 5];
        }
        cached_n = n;
    }

    return cache[n];
}
