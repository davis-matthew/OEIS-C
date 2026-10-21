/*
 * A283820  a(n) = (1 + Sum_{j=1..K-2} a(n-j)*a(n-j-1))/a(n-K) with
 *          a(1),...,a(K)=1, where K=7.
 * https://oeis.org/A283820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 13, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A283820
 *
 *  Recurrence:
 *      a(n) = (1 + Σ_{j=1..5} a(n-j)*a(n-j-1)) / a(n-7)
 *      a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=a(7)=1
 *
 *  The function is memoised so that calls may be in any order.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* array indexed from 0..max_n, value at index n is a(n) */
static size_t cache_cap = 0;     /* allocated capacity (elements) */
static size_t cache_len = 0;     /* largest n for which a(n) is stored */

/* Ensure cache can hold indices up to n  (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* out of range, not used */

    if (n > cache_len)
    {
        ensure_capacity(n);
        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            if (i <= 7)
                cache[i] = 1;
            else
            {
                seq_t sum = 1;
                for (int j = 1; j <= 5; ++j)          /* K-2 = 5 */
                    sum += cache[i - j] * cache[i - j - 1];
                cache[i] = sum / cache[i - 7];
            }
        }
        cache_len = n;
    }
    return cache[n];
}
