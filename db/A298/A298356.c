/*
 * A298356  a(n) = a(n-1) + a(n-2) + a([n/2]) + a([n/3]) + ... + a([n/n]),
 *          where a(0) = 1, a(1) = 1, a(2) = 1.
 * https://oeis.org/A298356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + sum_{k=2}^{n} a(floor(n/k)), a(0)=1,
 *            a(1)=1, a(2)=1
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;      /* capacity of the cache array           */
static idx_t  computed = 0;        /* number of terms actually computed     */

static void
ensure_cache(size_t n)            /* make sure cache has room for indices 0..n-1 */
{
    if (cache_size >= n)
        return;
    size_t new_size = cache_size ? cache_size : 1;
    while (new_size < n)
        new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();                  /* OOM; the test harness will not hit this */
    cache = tmp;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 3)                     /* base cases */
        return 1;

    /* make sure we have space for at least n+1 entries (0 .. n)   */
    ensure_cache(n + 1);

    /* initialise the first three terms on the first call or when the
       cache is resized past them                                                          */
    if (computed < 3) {
        cache[0] = cache[1] = cache[2] = 1;
        computed = 3;
    }

    /* compute any missing terms up to n */
    for (idx_t cur = computed; cur <= n; ++cur) {
        seq_t total = 0;
        idx_t k = 2;
        while (k <= cur) {
            idx_t t = cur / k;                    /* floor(cur/k)   */
            idx_t next_k = cur / t + 1;           /* first k with a different floor value */
            idx_t count = next_k - k;             /* how many terms share this floor value */
            total += cache[t] * (seq_t)count;
            k = next_k;
        }
        cache[cur] = cache[cur-1] + cache[cur-2] + total;
    }
    computed = n + 1;

    return cache[n];
}
