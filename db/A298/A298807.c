/*
 * A298807  Growth series for group with presentation < S, T : S^3 = T^3 =
 *          (S*T)^6 = 1 >.
 * https://oeis.org/A298807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 32, u64 n <= 65, u128 n <= 131,
 *            big any n
 * Helpers:   alloc_seq_array, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2)+2*a(n-3)+3*a(n-4)+5*a(n-5)+3*a(n-6)+2*a(n-7)+a(n-8)-a(n-10)
   for n>11, with initial values
   a(0)=1,a(1)=4,a(2)=8,a(3)=16,a(4)=32,a(5)=64,a(6)=126,a(7)=242,a(8)=472,
   a(9)=920,a(10)=1792,a(11)=3486.  */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq_array.h"

static const seq_t base_vals[12] = {1, 4, 8, 16, 32, 64, 126, 242, 472, 920, 1792, 3486};

static seq_t   *cache  = NULL;   /* array of computed values          */
static idx_t    cache_len = 0;   /* allocated length of cache         */
static idx_t    computed_up_to = (idx_t)-1; /* last index computed */

seq_t a(idx_t n)
{
    /* Ensure cache exists and contains the first 12 terms. */
    if (cache == NULL) {
        cache_len = 12;
        cache = alloc_seq_array(cache_len);
        for (idx_t i = 0; i < 12; ++i) {
            cache[i] = base_vals[i];
        }
        computed_up_to = 11;
    }
    /* Expand cache if necessary. */
    if (n + 1 > cache_len) {
        grow_seq_array(&cache, &cache_len, n + 1);
    }
    /* Compute missing values up to n. */
    if (n > computed_up_to) {
        idx_t start = computed_up_to + 1;
        if (start < 12) {
            /* If start is before the recurrence zone, fill with base values. */
            for (idx_t i = start; i < 12 && i <= n; ++i) {
                cache[i] = base_vals[i];
            }
            start = 12;
        }
        for (idx_t i = start; i <= n; ++i) {
            /* recurrence: a(i) = a(i-2)+2*a(i-3)+3*a(i-4)+5*a(i-5)+3*a(i-6)+2*a(i-7)+a(i-8)-a(i-10) */
            seq_t val = cache[i - 2];
            val += cache[i - 3] * 2;
            val += cache[i - 4] * 3;
            val += cache[i - 5] * 5;
            val += cache[i - 6] * 3;
            val += cache[i - 7] * 2;
            val += cache[i - 8];
            val -= cache[i - 10];
            cache[i] = val;
        }
        computed_up_to = n;
    }
    return cache[n];
}
