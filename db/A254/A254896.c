/*
 * A254896  Octagonal numbers (A000567) that are also centered square numbers
 *          (A001844).
 * https://oeis.org/A254896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+9602*a(n-2)-9602*a(n-3)-a(n-4)+a(n-5) (n>=6)
 *            Initial values: a(1)=1, a(2)=481, a(3)=8321, a(4)=4617761,
 *            a(5)=79897441
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Ensure cache holds terms up to index n (1‑based) */
static void ensure_cache(idx_t n)
{
    if (n == 0 || (size_t)n <= cache_len) return;

    size_t old_len = cache_len;
    cache_len = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    const seq_t init[5] = { (seq_t)1, (seq_t)481, (seq_t)8321,
                            (seq_t)4617761, (seq_t)79897441 };
    const seq_t C = 9602;

    for (size_t i = old_len; i < cache_len; ++i) {
        if (i < 5) {
            /* initial terms for i=0..4 (corresponding to n=1..5) */
            cache[i] = init[i];
        } else {
            /* recurrence: a(i+1) = a(i)+9602*a(i-1)-9602*a(i-2)-a(i-3)+a(i-4) */
            seq_t term1 = cache[i-1];
            seq_t term2 = C * cache[i-2];
            seq_t term3 = C * cache[i-3];
            seq_t term4 = cache[i-4];
            seq_t term5 = cache[i-5];
            cache[i] = term1 + term2 - term3 - term4 + term5;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
