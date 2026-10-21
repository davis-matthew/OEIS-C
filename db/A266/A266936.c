/*
 * A266936  Number of 3 X n binary arrays with rows lexicographically
 *          nondecreasing and columns lexicographically nondecreasing and row
 *          sums nondecreasing and column sums nonincreasing.
 * https://oeis.org/A266936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-4)-a(n-5)+a(n-6) for n ≥ 7 * Initial
 *            values: a(1)=4, a(2)=7, a(3)=9, a(4)=12, a(5)=14, a(6)=19
 * Range:     u8 n <= 34, u16 n <= 158, u32 n <= 158, u64 n <= 158,
 *            u128 n <= 158, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* array of computed terms  */
static idx_t cache_sz = 0;           /* current length of cache */ 

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* safety, though n ≥ 1 by definition */

    /* enlarge cache if necessary */
    if (n > cache_sz) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (size_t)n * sizeof(seq_t));
        if (!new_cache) {          /* allocation failed – abort */
            return 0;
        }
        cache = new_cache;

        /* fill newly allocated entries */
        for (idx_t i = cache_sz; i < n; ++i) {
            if (i < 6) {  /* base values a(1) … a(6) */
                static const seq_t init[6] = {
                    (seq_t)4, (seq_t)7, (seq_t)9,
                    (seq_t)12, (seq_t)14, (seq_t)19
                };
                cache[i] = init[i];
            } else {     /* recurrence for i ≥ 6 (n ≥ 7)   */
                /* indexes: a(n-1)=cache[i-1], a(n-2)=cache[i-2],
                 * a(n-4)=cache[i-4], a(n-5)=cache[i-5], a(n-6)=cache[i-6] */
                seq_t sum = cache[i-1] + cache[i-2] + cache[i-6];
                seq_t diff = cache[i-4] + cache[i-5];
                cache[i] = sum - diff;
            }
        }
        cache_sz = n;
    }

    return cache[n-1];  /* cache[0] = a(1) */
}
