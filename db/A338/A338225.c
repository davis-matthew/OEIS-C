/*
 * A338225  a(n) = F(n+3) * F(n+1) + (-1)^n where F(n) = A000045(n) are the
 *          Fibonacci numbers.
 * https://oeis.org/A338225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n+3) * F(n+1) + (-1)^n
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2, a(2)=11, a(3)=23, a(n)=2*a(n-1)+2*a(n-2)-a(n-3) for n>3 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (cache_len < n)
    {
        idx_t old_len = cache_len;
        cache_len = n;
        seq_t *tmp = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        cache = tmp;
        if (cache_len >= 1) cache[0] = (seq_t)2;
        if (cache_len >= 2) cache[1] = (seq_t)11;
        if (cache_len >= 3) cache[2] = (seq_t)23;
        for (idx_t i = old_len; i < cache_len; ++i)
        {
            if (i < 3) continue;
            cache[i] = cache[i-1] * (seq_t)2 + cache[i-2] * (seq_t)2 - cache[i-3];
        }
    }
    return cache[n-1];
}
