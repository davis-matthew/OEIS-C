/*
 * A243821  Three-column table read by rows: coefficient table for generalized
 *          m-nomial transforms of Fibonacci numbers (A000045).
 * https://oeis.org/A243821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence * a(0)=2, a(1)=1, a(2)=0 * a(n) = 2*a(n-3) - a(n-9)
 *            for n%3==0, n>=9 * a(n) = a(n-4) - a(n-3) for n%3==1, n>=4 *
 *            a(n) = fib(floor(n/3)+2) - 1 for n%3==2 * * The function uses
 *            memoisation so that any n ≥ 0 can be queried in * O(n) time and
 *            O(n) space overall. It works for all build‐time...
 * Range:     u8 n <= 26, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/fib.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;

static const seq_t init_vals[9] = {
    (seq_t)2, (seq_t)1, (seq_t)0,
    (seq_t)5, (seq_t)1, (seq_t)1,
    (seq_t)9, (seq_t)4, (seq_t)2
};

static void ensure_cache(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= cache_len)
        return;

    seq_t *new_cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
    if (!new_cache) abort();

    size_t old_len = cache_len;
    cache = new_cache;
    cache_len = required;

    for (size_t i = old_len; i < required; ++i) {
        if (i < 9) {
            cache[i] = init_vals[i];
        } else {
            if (i % 3 == 0) {
                /* i ≥ 9 and i % 3 == 0 */
                cache[i] = (seq_t)2 * cache[i-3] - cache[i-9];
            } else if (i % 3 == 1) {
                /* i ≥ 4 and i % 3 == 1 */
                cache[i] = cache[i-4] - cache[i-3];
            } else { /* i % 3 == 2 */
                idx_t m = (idx_t)(i / 3) + 2;
                cache[i] = fib(m) - (seq_t)1;
            }
        }
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
