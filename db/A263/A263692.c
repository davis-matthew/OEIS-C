/*
 * A263692  Number of length n arrays of permutations of 0..n-1 with each
 *          element moved by -3 to 3 places and every three consecutive
 *          elements having its maximum within 3 of its minimum.
 * https://oeis.org/A263692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2, a(3) = 6, a(4) = 24, a(5) = 18, a(6) = 18,
 *            a(7) = 24, a(8) = 40, a(9) = 64, a(10) = 100, a(11) = 144, a(12)
 *            = 216 For n > 12: a(n) = a(n-1) + a(n-2) – a(n-3) + a(n-4)
 *            (index starts at 1)
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 52, u64 n <= 106,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Thread‑safe static cache (no multi‑threading support required). */
static seq_t *cache = NULL;     /* pointer to array of computed terms */
static size_t cache_len = 0;    /* number of terms currently stored (a(1) stored at cache[0]) */

seq_t a(idx_t n)
{
    /* n is 1‑based; 0 is a guard value. */
    if (n == 0)
        return (seq_t)0;

    /* Target size (1‑based → index-0) */
    size_t target = (size_t)n;

    /* Grow the cache if the requested term has not yet been computed. */
    if (target > cache_len) {
        /* Allocate or reallocate memory. In C++ we cast explicitly. */
        size_t new_cap = target;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0 as a safe failure value. */
            free(cache);
            cache = NULL;
            cache_len = 0;
            return (seq_t)0;
        }
        cache = tmp;

        /* Compute new entries. */
        for (size_t i = cache_len; i < new_cap; ++i) {
            if (i < 12) {          /* initial values */
                switch (i) {       /* indices 0..11 correspond to n=1..12 */
                case 0: cache[i] = (seq_t)1;   break;
                case 1: cache[i] = (seq_t)2;   break;
                case 2: cache[i] = (seq_t)6;   break;
                case 3: cache[i] = (seq_t)24;  break;
                case 4: cache[i] = (seq_t)18;  break;
                case 5: cache[i] = (seq_t)18;  break;
                case 6: cache[i] = (seq_t)24;  break;
                case 7: cache[i] = (seq_t)40;  break;
                case 8: cache[i] = (seq_t)64;  break;
                case 9: cache[i] = (seq_t)100; break;
                case 10: cache[i] = (seq_t)144; break;
                case 11: cache[i] = (seq_t)216; break;
                }
            } else { /* recurrence for n > 12 */
                /* i corresponds to n=i+1, so a(n) uses a(i), a(i-1), a(i-2), a(i-3) */
                cache[i] = cache[i-1] + cache[i-2] - cache[i-3] + cache[i-4];
            }
        }
        cache_len = new_cap;
    }

    /* Return the requested term. */
    return cache[target - 1];
}
