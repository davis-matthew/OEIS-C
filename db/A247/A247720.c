/*
 * A247720  Number of length n+3 0..2 arrays with no disjoint pairs in any
 *          consecutive four terms having the same sum.
 * https://oeis.org/A247720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n‑1) + 2*a(n‑4) – 4*a(n‑5) (n ≥ 6) initial values:
 *            a(1)=48, a(2)=90, a(3)=172, a(4)=334, a(5)=656 offset: 1 (a(1)
 *            is the first term)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array */
#include "../utils/free_seq_array.h"    /* free_seq_array */

seq_t a(idx_t n)
{
    /* cache[0] is unused; cache indices 1.. are sequence values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of slots allocated   */

    if (n == 0)          /* offset is 1; callers may pass 0 by mistake */
        return 0;

    if (n >= cache_size) {
        /* Allocate a larger array that holds indices up to n.   */
        idx_t new_size = n + 1;                     /* indices 0..n */
        seq_t *new_cache = alloc_seq_array(new_size);

        /* Copy any already‑computed values.                 */
        if (cache_size > 0) {
            for (idx_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
        }

        /* Initialise new entries, starting from the first free
           slot (cache_size).  For new_size==0 nothing happens. */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                new_cache[i] = 0;                     /* unused slot */
            } else if (i == 1) {
                new_cache[i] = 48;
            } else if (i == 2) {
                new_cache[i] = 90;
            } else if (i == 3) {
                new_cache[i] = 172;
            } else if (i == 4) {
                new_cache[i] = 334;
            } else if (i == 5) {
                new_cache[i] = 656;
            } else {      /* recurrence: n ≥ 6 */
                new_cache[i] = (seq_t)2 * new_cache[i-1]
                             + (seq_t)2 * new_cache[i-4]
                             - (seq_t)4 * new_cache[i-5];
            }
        }

        cache      = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
