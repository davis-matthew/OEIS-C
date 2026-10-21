/*
 * A251145  Number of (n+1) X (4+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to 4 and no 2 X 2 subblock having exactly two nonzero
 *          entries.
 * https://oeis.org/A251145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A251145: Number of (n+1)×5 arrays over {0,1,2} with every 2×2 subblock
 * summing to 4 and no 2×2 subblock having exactly two nonzero entries.
 *
 * Recurrence (for n ≥ 6):
 *   a(n) = 3*a(n-1) - 5*a(n-3) + a(n-4) + 2*a(n-5)
 *
 * Initial values:
 *   a(1) = 107
 *   a(2) = 161
 *   a(3) = 247
 *   a(4) = 401
 *   a(5) = 667
 *
 * The function below memoises values in a static dynamically‑resized
 * array so that any call order is supported.  It works for both builtin
 * integer types and arbitrary‑precision integer classes.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of values stored */

    /* initial values (index 1 → cache[0]) */
    static const seq_t init[5] = {107, 161, 247, 401, 667};

    if (n < 1)
        return 0;                /* undefined for n=0, harmless */

    /* grow cache if needed */
    if (cache_len < n) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();      /* out of memory */
        cache = tmp;

        /* compute new entries */
        for (idx_t i = cache_len + 1; i <= new_len; ++i) {
            if (i <= 5) {
                cache[i - 1] = init[i - 1];
            } else {
                /* recurrence: a(n) = 3*a(n-1) - 5*a(n-3) + a(n-4) + 2*a(n-5) */
                seq_t val = 3 * cache[i - 2];      /* 3*a(i-1)   */
                val -= 5 * cache[i - 4];          /* -5*a(i-3)  */
                val += cache[i - 5];              /* + a(i-4)   */
                val += 2 * cache[i - 6];          /* +2*a(i-5)  */
                cache[i - 1] = val;
            }
        }
        cache_len = new_len;
    }
    return cache[n - 1];
}
