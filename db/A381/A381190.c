/*
 * A381190  Number of connected minimal dominating sets in the n-trapezohedral
 *          graph.
 * https://oeis.org/A381190
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-2)+2*a(n-3)-a(n-4)-2*a(n-5)-a(n-6),
   with initial values a(3)=6, a(4)=16, a(5)=30, a(6)=36, a(7)=70, a(8)=96.
   For n < 3 the function returns 0. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    static seq_t *cache = NULL;            /* cache[i] holds a(3 + i) */
    static idx_t cache_len = 0;            /* number of stored values */

    /* Compute up to required index if not already cached */
    idx_t required_len = n - 2;            /* indices 0 .. n-3 inclusive */
    if (cache_len < required_len) {
        size_t new_len = (size_t)required_len;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) /* allocation failed */
            return 0;
        cache = tmp;

        /* Base values for a(3)..a(8) */
        static const seq_t base[6] = { (seq_t)6, (seq_t)16, (seq_t)30,
                                       (seq_t)36, (seq_t)70, (seq_t)96 };

        for (idx_t i = cache_len; i < required_len; ++i) {
            idx_t cur_n = 3 + i; /* current n */
            seq_t val;
            if (cur_n <= 8) { /* use base table */
                val = base[cur_n - 3];
            } else { /* recurrence */
                seq_t a_nm2 = cache[(size_t)(cur_n - 5)]; /* a(n-2) */
                seq_t a_nm3 = cache[(size_t)(cur_n - 6)]; /* a(n-3) */
                seq_t a_nm4 = cache[(size_t)(cur_n - 7)]; /* a(n-4) */
                seq_t a_nm5 = cache[(size_t)(cur_n - 8)]; /* a(n-5) */
                seq_t a_nm6 = cache[(size_t)(cur_n - 9)]; /* a(n-6) */

                val = a_nm2 + a_nm2;      /* 2*a(n-2) */
                val += a_nm3 + a_nm3;      /* +2*a(n-3) */
                val -= a_nm4;              /* - a(n-4) */
                val -= a_nm5 + a_nm5;      /* -2*a(n-5) */
                val -= a_nm6;              /* - a(n-6) */
            }
            cache[i] = val;
        }
        cache_len = required_len;
    }

    return cache[(size_t)(n - 3)];
}
