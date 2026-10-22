/*
 * A367717  G.f. A(x) satisfies A(x) = 1 / ((1 + x) * (1 - x * (1 + x + x^2) *
 *          A(x^3))).
 * https://oeis.org/A367717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + Sum_{k=0..n-1} a(floor(k/3)) * a(n-1-k)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* memoization buffer */
static size_t cache_cap  = 0;      /* current capacity (number of elements) */
static idx_t  max_index  = (idx_t)-1; /* highest index that has been computed */

/* Ensure the buffer can hold indices 0..n.  The buffer grows to at least n+1
   elements, doubling its size each time to amortise allocations. */
static void grow_cache(idx_t n)
{
    if (cache_cap > (size_t)n) return;

    size_t new_cap = cache_cap == 0 ? (size_t)n + 1 : cache_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;               /* realloc(NULL, sz) behaves like malloc(sz) */
    cache_cap = new_cap;
}

/* Public sequence routine – external linkage as requested. */
seq_t a(idx_t n)
{
    /* Get a buffer big enough for all indexes up to n. */
    grow_cache(n);

    /* Initialise base cases on the first invocation. */
    if (max_index == (idx_t)-1) {
        cache[0] = (seq_t)1;        /* a(0) */
        max_index = 0;
        if (n >= 1) {               /* a(1) = 0 */
            cache[1] = (seq_t)0;
            max_index = 1;
        }
    }

    /* Compute values that have not yet been cached yet. */
    if (n > max_index) {
        for (idx_t i = max_index + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; k < i; ++k) {
                idx_t f = k / 3;                 /* floor(k/3) */
                sum += cache[f] * cache[i - 1 - k];
            }
            /* Add the (-1)^i term. */
            if ((i & 1) == 0) sum += (seq_t)1;
            else              sum -= (seq_t)1;

            cache[i] = sum;
        }
        max_index = n;
    }

    return cache[n];
}
