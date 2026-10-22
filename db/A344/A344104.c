/*
 * A344104  a(0) = 10; for n > 0, a(n) is a(n-1) multiplied by the number of
 *          0's so far in the sequence.
 * https://oeis.org/A344104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=10; for n>0, a(n)=a(n-1) multiplied by the number
   of 0's seen in a(n-1), a(n-2), …, a(0).  Z(k) denotes the total
   number of 0's among the first k+1 terms.  a(n)=a(n-1)*Z(n-1). */

#include <stdlib.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    /* In-cache sequence values */
    static seq_t *seq_cache  = NULL;   /* a(i)   */
    /* In-cache cumulative zero counts up to i */
    static idx_t *zero_cache = NULL;   /* sum zeros up to i */
    static size_t cache_len = 0;       /* next free index + 0 */

    /* Grow the caches if needed */
    if ((idx_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;

        seq_cache  = (seq_t *)realloc(seq_cache,
                                      new_len * sizeof(*seq_cache));
        zero_cache = (idx_t *)realloc(zero_cache,
                                      new_len * sizeof(*zero_cache));
        if (!seq_cache || !zero_cache)   /* allocation failure */
            exit(1);

        for (idx_t i = cache_len; i <= (idx_t)n; ++i) {
            unsigned char cnt[10] = {0};

            if (i == 0) {
                seq_cache[0] = (seq_t)10;
                digit_counts(seq_cache[0], cnt);
                zero_cache[0] = cnt[0];
            } else {
                seq_t prev_val = seq_cache[i - 1];
                idx_t prev_zeros = zero_cache[i - 1];

                seq_t cur_val = prev_val * (seq_t)prev_zeros;
                seq_cache[i] = cur_val;

                digit_counts(cur_val, cnt);
                zero_cache[i] = prev_zeros + (idx_t)cnt[0];
            }
        }
        cache_len = new_len;
    }
    return seq_cache[n];
}
