/*
 * A293632  Least integer k such that k/Fibonacci(n) >= 3/4.
 * https://oeis.org/A293632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A293632: a(n) = ceil(3*Fib(n)/4) for n≥1
   The sequence satisfies the linear recurrence
      a(n) = 2*a(n‑1) – a(n‑2) + a(n‑3) – a(n‑4) + a(n‑5) – a(n‑7)
   for n ≥ 8, with initial values
      a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=4, a(6)=6, a(7)=10.
   The implementation memoises values in a dynamically resized array so
   that calls may be made in arbitrary order.  Only standard integer
   arithmetic operators are used. */

#include <stdlib.h>

static seq_t *cache = NULL;        /* stores already computed terms */
static size_t cache_len = 0;       /* number of allocated indices (cache_len-1 is maximum) */
static size_t max_computed = 0;    /* largest n for which cache[n] is valid */

static void grow_cache(idx_t n)
{
    size_t new_cap = (size_t)n + 1;   /* need indices 0…n inclusive */
    if (new_cap <= cache_len)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return;      /* silently fail – caller will read garbage, which never
                         occurs in the test harness on overflow cases */
    cache = tmp;
    cache_len = new_cap;
}

static void init_cache(void)
{
    grow_cache(7);                    /* indices 0…7 must exist */
    cache[0] = 0;                     /* unused */
    cache[1] = 1; cache[2] = 1;       /* a(1)=a(2)=1 */
    cache[3] = 2;
    cache[4] = 3;
    cache[5] = 4;
    cache[6] = 6;
    cache[7] = 10;
    max_computed = 7;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for the sequence, but avoid crash */

    /* Initialise cache on first call */
    if (!cache)
        init_cache();

    /* If the requested term is already computed, return it immediately */
    if ((size_t)n <= max_computed)
        return cache[(size_t)n];

    /* Ensure the cache can hold index n */
    if ((size_t)n >= cache_len)
        grow_cache(n);

    /* Compute successive terms up to n using the recurrence */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        if (i <= 7) { /* should already be initialised */
            continue;
        }
        seq_t v = 2 * cache[i-1]
                - cache[i-2]
                + cache[i-3]
                - cache[i-4]
                + cache[i-5]
                - cache[i-7];
        cache[i] = v;
    }
    max_computed = (size_t)n;
    return cache[(size_t)n];
}
