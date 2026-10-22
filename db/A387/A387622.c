/*
 * A387622  a(n) = Sum_{k=0..floor(n/3)} 2^k * binomial(2*n-4*k,2*k).
 * https://oeis.org/A387622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A387622:
 *   a(n) = Sum_{k=0..⌊n/3⌋} 2^k * binomial(2n-4k, 2k)
 *
 * Recurrence (valid for n ≥ 6):
 *   a(0)=1, a(1)=1, a(2)=1, a(3)=3, a(4)=13, a(5)=31
 *   a(n) = 2*a(n-1) - a(n-2) + 4*a(n-3) + 4*a(n-4) - 4*a(n-6)
 *
 * The function below memoizes previously computed values so that it can
 * be called in any order.  It uses only standard C11 constructs and
 * works with any of the supported `seq_t` or `idx_t` types.
 */

#include <stdlib.h>
#include <string.h>

/* Function: seq_t a(idx_t n)
 * Computes the nth term of OEIS sequence A387622.
 * seq_t and idx_t are defined by the build system.
 */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements already computed */
    static idx_t cache_cap  = 0;   /* allocated capacity */

    /* enlarge cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= n)
            new_cap <<= 1;               /* double until >= n+1 */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!new_cache) { /* allocation failed: abort */
            abort();
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute values up to n if not already present */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i == 0)          val = (seq_t)1;
        else if (i == 1)     val = (seq_t)1;
        else if (i == 2)     val = (seq_t)1;
        else if (i == 3)     val = (seq_t)3;
        else if (i == 4)     val = (seq_t)13;
        else if (i == 5)     val = (seq_t)31;
        else { /* i >= 6 */
            /* recurrence: a(n) = 2*a(n-1) - a(n-2) + 4*a(n-3) + 4*a(n-4) - 4*a(n-6) */
            val = (seq_t)( 2 * cache[i-1]
                         - cache[i-2]
                         + 4 * cache[i-3]
                         + 4 * cache[i-4]
                         - 4 * cache[i-6] );
        }
        cache[i] = val;
    }
    cache_size = n + 1;
    return cache[n];
}
