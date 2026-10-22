/*
 * A372205  a(n) = (-1)^n*a((n - 2^A007814(n))/2) + a(floor((2*n -
 *          2^A007814(n))/2)) for n > 0 and a(0) = 1.
 * https://oeis.org/A372205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n > 0: let m = lowbit(n) = n & (~n + 1) // 2^v2(n)
 *            let r = n - m if n is even: a(n) = a(r/2) + a(r + m/2) if n is
 *            odd : a(n) = a(r + m/2) - a(r/2)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static bool   *computed = NULL;
static size_t  cache_len = 0;

/* Ensure cache has room for indices up to n, growing if necessary. */
static void ensure_capacity(idx_t n)
{
    if (!cache) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        computed = (bool *)malloc(sizeof(bool) * cache_len);
        cache[0] = (seq_t)1;
        computed[0] = true;
    }
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n * 2 + 2;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        bool *new_comp = (bool *)realloc(computed, sizeof(bool) * new_len);
        memset(new_cache + cache_len, 0, sizeof(seq_t) * (new_len - cache_len));
        memset(new_comp + cache_len, 0, sizeof(bool) * (new_len - cache_len));
        cache = new_cache;
        computed = new_comp;
        cache_len = new_len;
    }
}

/* Compute all a(i) for i <= n, using the recurrence and the cache. */
static void compute_up_to(idx_t n)
{
    ensure_capacity(n);
    for (idx_t i = 1; i <= n; ++i) {
        if (computed[i])
            continue;
        /* lowbit(n) = n & (~n + 1) */
        idx_t m = i & (~i + 1);
        idx_t r = i - m;
        seq_t first  = cache[r / 2];
        seq_t second = cache[r + (m >> 1)];
        if (i & 1)          /* odd: second - first */
            cache[i] = second - first;
        else                /* even: first + second */
            cache[i] = first + second;
        computed[i] = true;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    compute_up_to(n);
    return cache[n];
}
