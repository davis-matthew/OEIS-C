/*
 * A247049  Rectangular array read upwards by columns: T = T(n,k) = number of
 *          paths from (0,0) to (n,k), where 0 <= k <= 2, consisting of
 *          segments given by the vectors (1,1), (1,2), (1,-1).
 * https://oeis.org/A247049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = a(n-3) + a(n-6), with a(0)=1, a(1)=0, a(2)=0, a(3)=0,
 *            a(4)=1, a(5)=1
 * Range:     u8 n <= 42, u16 n <= 75, u32 n <= 144, u64 n <= 282,
 *            u128 n <= 561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;      /* table of computed values      */
static size_t cache_len = 0;     /* number of elements stored     */
static size_t cache_cap = 0;     /* allocated capacity             */

/* enlarge the cache so that cache[n] is a valid location            */
static void ensure_capacity(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    if (new_cap < required) new_cap = required;

    if (!cache)
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    cache_cap = new_cap;
}

/* compute all values up to distance n (inclusive) and store them   */
static void compute_up_to(idx_t n)
{
    ensure_capacity(n);
    while (cache_len <= (size_t)n)
    {
        size_t i = cache_len;
        seq_t val;
        if (i == 0)       val = (seq_t)1;
        else if (i == 1)  val = (seq_t)0;
        else if (i == 2)  val = (seq_t)0;
        else if (i == 3)  val = (seq_t)0;
        else if (i == 4)  val = (seq_t)1;
        else if (i == 5)  val = (seq_t)1;
        else              val = cache[i-3] + cache[i-6];
        cache[cache_len++] = val;
    }
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[(size_t)n];
}
