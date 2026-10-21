/*
 * A290925  p-INVERT of the positive integers, where p(S) = 1 - 3*S - 2*S^2.
 * https://oeis.org/A290925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n‑1) - 10*a(n‑2) + 7*a(n‑3) - a(n‑4) a(0) = 3, a(1) =
 *            17, a(2) = 92, a(3) = 495
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache of computed terms (0‑based index) */
static seq_t *cache = NULL;        /* Allocated array of terms */
static size_t cache_size  = 0;     /* Allocated size of the array   */
static size_t cache_used  = 0;     /* Number of terms actually stored */

/* Initialise the first four known terms if necessary */
static void init_cache(void)
{
    if (cache_used < 4) {
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)17;
        cache[2] = (seq_t)92;
        cache[3] = (seq_t)495;
        cache_used = 4;
    }
}

/* Grow the cache to at least *new_cap* elements (power‑of‑two). */
static void grow_cache_to(size_t new_cap)
{
    if (new_cap <= cache_size) return;
    size_t cap = cache_size ? cache_size : 1;
    while (cap < new_cap) cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
    if (!tmp) abort();                /* out of memory */
    cache = tmp;
    cache_size = cap;
}

/* Ensure that all terms up to index n (inclusive) are stored in cache. */
static void ensure_cache_up_to(idx_t n)
{
    size_t target = (size_t)n + 1;
    if (target <= cache_used) return;       /* already computed */

    /* Make sure we have room for the new terms */
    if (target > cache_size) grow_cache_to(target);

    /* Initialise the first four terms if not yet done */
    init_cache();

    /* Compute any new terms using the recurrence */
    for (size_t i = cache_used; i < target; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        cache[i] = 7 * a1 - 10 * a2 + 7 * a3 - a4;
    }
    cache_used = target;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Handle the first four terms without allocating the cache */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)17;
    if (n == 2) return (seq_t)92;
    if (n == 3) return (seq_t)495;

    /* Compute or retrieve the requested term */
    ensure_cache_up_to(n);
    return cache[(size_t)n];
}
