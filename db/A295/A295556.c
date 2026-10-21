/*
 * A295556  a(n) = 0 for n <= 1; thereafter a(n) = a(floor(n/2)) +
 *          a(ceiling(n/2)) + floor(n/2) if n not congruent to 0 mod 4, a(n) =
 *          a(n/2-1) + a(n/2+1) + n/2 if n == 0 (mod 4).
 * https://oeis.org/A295556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=0; For n > 1: if n % 4 ≠ 0: a(n) = a(floor(n/2)) +
 *            a(ceil(n/2)) + floor(n/2) if n % 4 = 0: a(n) = a(n/2-1) +
 *            a(n/2+1) + n/2
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static bool  *seen    = NULL;
    static size_t cache_len = 0;

    /* Resize cache if necessary.  Cast the realloc result to match
       the pointer type for C++ compatibility. */
    if ((size_t)n >= cache_len)
    {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        seen  = (bool  *)realloc(seen,  new_len * sizeof(bool));
        for (size_t i = cache_len; i < new_len; ++i)
        {
            cache[i] = 0;
            seen[i]  = false;
        }
        cache_len = new_len;
    }

    if (seen[n])
        return cache[n];

    /* Base cases. */
    if (n <= 1)
    {
        cache[n] = 0;
        seen[n]  = true;
        return 0;
    }

    seq_t result;
    if (n & 3)            /* n % 4 != 0 */
    {
        idx_t f = n >> 1;            /* floor(n/2) */
        idx_t c = (n + 1) >> 1;      /* ceil(n/2)  */
        result = a(f) + a(c) + (seq_t)f;
    }
    else                    /* n % 4 == 0 */
    {
        idx_t h = n >> 1;            /* n/2 */
        result = a(h - 1) + a(h + 1) + (seq_t)h;
    }

    cache[n] = result;
    seen[n]  = true;
    return result;
}
