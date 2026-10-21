/*
 * A284532  The lexicographically earliest sequence of positive integers such
 *          that a(1) = a(2) = 1 and a(n + k) != a(n - k) for all k <= a(n).
 * https://oeis.org/A284532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 523, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence with the constraint
 *   a(n + k) != a(n - k)  for all 1 ≤ k ≤ a(n).
 * The sequence starts a(1) = a(2) = 1.
 *
 * The implementation keeps a static cache and fills terms
 * greedily: for each new index i we choose the smallest
 * positive integer that does not violate any existing
 * inequality constraint involving i.
 * Complexity is O(n²) and satisfies the time constraint
 * for n up to about 10 000. */

#include <stdlib.h>
#include <stdbool.h>

/* Dynamic array helpers that work with arbitrary-precision seq_t. */
#include "../utils/alloc_seq_array.h"   /* seq_alloc_array(size_t)   */
#include "../utils/seq_realloc.h"       /* seq_realloc(seq_t*,size_t,size_t) */

/* Static cache state. */
static seq_t *cache        = NULL;  /* pointer to array of seq_t */
static idx_t capacity      = 0;     /* allocated size */
static idx_t size_cache    = 0;     /* number of terms stored */

/* Ensure that at least n terms can be stored. */
static void ensure_capacity(idx_t n)
{
    if (n <= capacity)
        return;

    size_t new_cap = capacity ? capacity * 2 : 4;
    while (new_cap < n)
        new_cap *= 2;

    /* seq_realloc correctly handles construction and destruction of
       non-trivial seq_t objects. */
    seq_t *tmp = seq_realloc(cache, capacity, new_cap);
    if (!tmp)
        abort();                /* memory error – terminate */
    cache = tmp;
    capacity = new_cap;
}

/* Initialise the cache with the first two terms. */
static void init_cache(void)
{
    if (cache)
        return;

    ensure_capacity(2);
    cache[0] = (seq_t)1;        /* a(1) = 1 */
    cache[1] = (seq_t)1;        /* a(2) = 1 */
    size_cache = 2;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    init_cache();

    if (n <= size_cache)
        return cache[n - 1];

    ensure_capacity(n);

    /* Compute missing terms from size_cache+1 up to n. */
    for (idx_t i = size_cache + 1; i <= n; ++i) {
        seq_t cand = (seq_t)1;

        /* Find the smallest candidate satisfying all existing constraints. */
        while (true) {
            bool ok = true;

            for (idx_t p = 1; p < i; ++p) {
                idx_t k = i - p;                /* distance from p to i */
                /* If a(p) permits a constraint that involves i,
                   we must ensure cand differs from a(p-k). */
                if ((seq_t)k <= cache[p - 1]) {
                    if (p > k) {                /* p-k ≥ 1 */
                        if (cand == cache[p - k - 1]) {
                            ok = false;
                            break;
                        }
                    }
                }
            }

            if (ok)
                break;
            ++cand;                             /* try next integer */
        }

        cache[i - 1] = cand;
    }

    size_cache = n;
    return cache[n - 1];
}
