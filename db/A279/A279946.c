/*
 * A279946  Numbers that are both dodecagonal and centered heptagonal.
 * https://oeis.org/A279946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(2) = 10396; a(3) = 326656; a(4) = 2619897841; for n
 *            > 4: a(n) = 252002*a(n-2) - a(n-4) + 85050
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* array of computed values, 1‑indexed */
static idx_t cache_up_to = 0;     /* highest index that has been computed */

/* Compute the sequence value for any n >= 1.  Results are memoised
   so that calls in arbitrary order are efficient. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n = 0, but not required */

    /* Allocate or extend cache up to n */
    if (n > cache_up_to) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failed */
        cache = tmp;

        /* Compute from the next index up to n */
        for (idx_t i = cache_up_to + 1; i <= n; ++i) {
            if (i == 1)          cache[i] = (seq_t)1;
            else if (i == 2)     cache[i] = (seq_t)10396;
            else if (i == 3)     cache[i] = (seq_t)326656;
            else if (i == 4)     cache[i] = (seq_t)2619897841;
            else {
                cache[i] = ((seq_t)252002 * cache[i-2]) - cache[i-4] + (seq_t)85050;
            }
        }
        cache_up_to = n;
    }
    return cache[n];
}
