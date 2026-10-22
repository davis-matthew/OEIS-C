/*
 * A370871  Numbers m such that c(0) >= c(1) > c(2), where c(k) = number of
 *          k's in the ternary representation of m.
 * https://oeis.org/A370871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer m such that in base‑3 representation, the
 *            counts satisfy c(0) >= c(1) > c(2) (where c(k) is the number of
 *            k's in the ternary digits of m)
 * Range:     u8 n <= 39, u16 n <= 9257, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;     /* allocated size of cache */
static size_t cache_len = 0;     /* number of elements stored */
static seq_t next_candidate = 1; /* next integer to test */

static void grow_cache(void)
{
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (tmp == NULL) {
        /* allocation failure – abort */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; ensure cache holds at least n elements */
    while (cache_len < n) {
        /* test next_candidate */
        seq_t m = next_candidate;
        next_candidate++;        /* increment for next round */

        /* count digits in base 3 */
        unsigned int c[3] = {0, 0, 0};
        seq_t tmp = m;
        if (tmp == 0) {
            c[0] = 1;
        } else {
            while (tmp != 0) {
                unsigned int d = (unsigned int)(tmp % 3);
                c[d]++;
                tmp /= 3;
            }
        }

        /* check condition c0 >= c1 && c1 > c2 */
        if (c[0] >= c[1] && c[1] > c[2]) {
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = m;
        }
    }
    return cache[n - 1];
}
