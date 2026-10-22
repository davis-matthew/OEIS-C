/*
 * A383304  Nonnegative integers whose difference between the largest and
 *          smallest digits is equal to the arithmetic mean of its digits.
 * https://oeis.org/A383304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: sequentially test nonnegative integers requiring
   (max_digit - min_digit) == arithmetic mean of digits. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static seq_t next = 0;

    if (n == (idx_t)0) return 0;                 /* safeguard */
    size_t target = (size_t)n;                  /* 1‑based index */

    while (cache_size < target)
    {
        seq_t cand = next;
        bool good = false;

        if (cand == (seq_t)0)                     /* 0 is a special case */
        {
            good = true;
        }
        else
        {
            seq_t temp = cand;
            seq_t sum = 0, min = (seq_t)9, max = 0, len = 0;

            while (temp > 0)
            {
                seq_t d = temp % 10;
                sum += d;
                if (d < min) min = d;
                if (d > max) max = d;
                len++;
                temp /= 10;
            }

            if (sum % len == 0 && (max - min) == sum / len)
                good = true;
        }

        if (good)
        {
            if (cache_size >= cache_cap)
            {
                if (cache_cap == 0) cache_cap = 16;
                else cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = cand;
        }

        next = cand + 1;
    }

    return cache[target - 1];
}
