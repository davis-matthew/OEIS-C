/*
 * A384336  a(1) = 1, a(2) = 2. For n > 2, a(n) = number of a(k), k = 1..n-2
 *          such that a(k) divides a(n-1).
 * https://oeis.org/A384336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1196, u16 n <= 8713, u32 n <= 8713, u64 n <= 8713,
 *            u128 n <= 8713, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2; for n>2,
   a(n)=# { k | 1≤k≤n-2, a(k) divides a(n-1) } */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* cache[i] holds a(i+1) for i>=0 */
static size_t cache_cap = 0;     /* current allocated capacity */
static idx_t cache_up_to = 0;    /* maximum n for which cache is valid */

/* ensure the cache contains values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_up_to)
        return;                       /* already computed */

    /* resize if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort for this sequence */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new terms */
    for (idx_t i = cache_up_to + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)1;
        } else if (i == 2) {
            cache[1] = (seq_t)2;
        } else {
            seq_t prev = cache[i-2];   /* a(n-1) */
            idx_t count = 0;
            for (idx_t k = 1; k <= i-2; ++k) {
                if (prev % cache[k-1] == 0)
                    ++count;
            }
            cache[i-1] = count;
        }
    }
    cache_up_to = n;
}

seq_t a(idx_t n)
{
    if (n == 0) {          /* out of range but harmless */
        return (seq_t)0;
    }
    ensure_cache(n);
    return cache[n-1];
}
