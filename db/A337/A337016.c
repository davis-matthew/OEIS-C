/*
 * A337016  a(0) = 0. Successive terms are double the previous, then with
 *          their digits incremented by 1.
 * https://oeis.org/A337016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 16, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0; for n>0, a(n) = A216556(2*a(n-1)) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t increment_and_double(seq_t prev)
{
    seq_t doubled = prev * 2;
    if (doubled == 0)
        return 1;                               /* 0 → 1 */

    /* Count number of decimal digits of doubled */
    size_t len = 0;
    seq_t tmp = doubled;
    do
    {
        ++len;
        tmp /= 10;
    } while (tmp > 0);

    /* Store digits in reverse order */
    uint8_t *rev = (uint8_t *)malloc(len);
    if (!rev)
        return 0;                               /* allocation failed */

    tmp = doubled;
    for (size_t pos = 0; pos < len; ++pos)
    {
        rev[pos] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* Build result: increment each digit, replace 9 by 10 */
    seq_t res = 0;
    for (size_t i = len; i-- > 0; )
    {
        uint8_t d = rev[i];
        if (d == 9)
        {
            res = res * (seq_t)10 + 1;
            res = res * (seq_t)10 + 0;
        }
        else
        {
            res = res * (seq_t)10 + (d + 1);
        }
    }

    free(rev);
    return res;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with a(0) */
    if (cache_size == 0)
    {
        cache = mem_new(seq_t, 1);
        if (!cache)
            return 0;                           /* allocation failed */
        cache[0] = 0;
        cache_size = 1;
    }

    /* fill cache up to index n if needed */
    if ((idx_t)n >= cache_size)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
        if (!new_cache)
            return 0;                           /* allocation failed */
        cache = new_cache;
        for (idx_t i = cache_size; i <= n; ++i)
            cache[i] = increment_and_double(cache[i - 1]);
        cache_size = new_cap;
    }

    return cache[n];
}
