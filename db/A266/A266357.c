/*
 * A266357  Number of n X 3 binary arrays with rows and columns
 *          lexicographically nondecreasing and row and column sums
 *          nondecreasing.
 * https://oeis.org/A266357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 5*a(n-1) -9*a(n-2) +6*a(n-3)
 *            -6*a(n-7) +9*a(n-8) -5*a(n-9) +a(n-10) Initial values: a(1)=4,
 *            a(2)=13, a(3)=35, a(4)=82, a(5)=173, a(6)=337, a(7)=614,
 *            a(8)=1060, a(9)=1749, a(10)=2777 The recurrence holds for all
 *            n>=11
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 141, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based cache of already computed values */
static idx_t cached_max = 0;         /* highest index that has been computed */
static const seq_t base_values[10] = {4, 13, 35, 82, 173, 337,
                                      614, 1060, 1749, 2777};

static void ensure_cache_size(idx_t n)
{
    /* Ensure that the cache array has room for indices 1 … n */
    if (n <= cached_max) return;
    size_t new_size = (size_t)n + 1; /* plus one for 0‑index dummy entry */
    if (!cache) {
        cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    }
    if (!cache) exit(1);   /* allocation failure */
    /* No need to initialize new elements; they will be filled below. */
}

static void compute_up_to(idx_t n)
{
    if (n==0) return;
    ensure_cache_size(n);
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[i] = base_values[i-1];
        } else {
            /* Compute a(i) via the recurrence, avoiding temporary
               negative values: sum of positive terms minus
               sum of negative terms.  The recurrence guarantees
               the result is non‑negative. */
            seq_t pos = (seq_t)5 * cache[i-1]
                      + (seq_t)6 * cache[i-3]
                      + (seq_t)9 * cache[i-8]
                      +            cache[i-10];
            seq_t neg = (seq_t)9 * cache[i-2]
                      + (seq_t)6 * cache[i-7]
                      + (seq_t)5 * cache[i-9];
            cache[i] = pos - neg;
        }
    }
    cached_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    compute_up_to(n);
    return cache[n];
}
