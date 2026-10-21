/*
 * A287430  Number of connected dominating sets in the 2n-crossed prism graph.
 * https://oeis.org/A287430
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A287430
   Formula: a(2) = 115
            a(n) = 7^n + 240*n*7^(n-3)  for n > 2
            Recurrence (for n > 4): a(n) = 14*a(n-1) - 49*a(n-2)
   Offset: 2
   Computes the number of connected dominating sets in the 2n‑crossed prism graph.
*/

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the offset: a(2) is the first valid term. */
    if (n < 2) return 0;

    /* Static storage for memoised results */
    static seq_t *cache = NULL;       /* array of computed terms */
    static idx_t  capacity = 0;       /* allocated size of cache */
    static idx_t  last_filled = 1;    /* largest index computed (<2 initially) */

    /* Pre‑compute small powers of 7 once */
    static seq_t pow7[6];
    static bool   pow7_ready = false;
    if (!pow7_ready) {
        pow7[0] = 1;
        for (int i = 1; i < 6; ++i) pow7[i] = pow7[i-1] * (seq_t)7;
        pow7_ready = true;
    }

    /* Allocate or grow the cache to fit n */
    if (!cache) {
        capacity = (n >= 16 ? n + 1ULL : 16ULL);
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!cache) return 0;  /* allocation failed – return 0 */
    } else if (capacity <= n) {
        idx_t new_cap = capacity;
        while (new_cap <= n) new_cap <<= 1;          /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;                          /* allocation failed */
        for (idx_t i = capacity; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last_filled + 1; i <= n; ++i) {
        seq_t val;
        if (i == 2) {
            val = 115;                                  /* base term */
        } else if (i >= 3 && i <= 5) {                    /* use closed form */
            val = pow7[i] + ((seq_t)240 * i) * pow7[i-3];
        } else {                                          /* recurrence */
            val = 14 * cache[i-1] - 49 * cache[i-2];
        }
        cache[i] = val;
    }
    last_filled = n;

    return cache[n];
}
