/*
 * A400375  Numbers whose greedy alternating digit sum is 0 (in base 10).
 * https://oeis.org/A400375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, new
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 5100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_good_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): Numbers whose greedy alternating digit sum is 0 (base 10).
   Computed by examining successive integers in ascending order and recording
   those that satisfy the alternating‑sum test. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_good_idx.h"

/* Returns the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0, len = 0;
    static idx_t next_num = 0;  /* next integer to test */

    if (n == 0)
        return 0;  /* not expected, defensive */

    while (len < n)
    {
        if (is_good_idx(next_num))
        {
            if (len >= cap)
            {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return 0;  /* allocation failed */
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = (seq_t)next_num;
        }
        ++next_num;
    }

    return cache[n - 1];
}
