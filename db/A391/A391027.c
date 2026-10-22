/*
 * A391027  Number of tetrahedra in the n X n white bishop graph.
 * https://oeis.org/A391027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*

OEIS A391027 – Number of tetrahedra in the n × n white bishop graph

Recurrence (valid for n ≥ 11):
    a(n) = 2*a(n-1) + 3*a(n-2) - 8*a(n-3) - 2*a(n-4)
         + 12*a(n-5) - 2*a(n-6) - 8*a(n-7) + 3*a(n-8)
         + 2*a(n-9)  - a(n-10)

Initial values (n = 1 … 10):
    a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=4, a(6)=27, a(7)=64,
    a(8)=182, a(9)=344, a(10)=714

The function stores computed terms in a static cache so that subsequent
calls with smaller or larger `n` are answered in O(1) or in O(max(n,prev_n))
time respectively.  All arithmetic is performed with the `seq_t` type,
so it works for the 8/16/32/64‑bit unsigned integer builds as well as
the arbitrary‑precision build.

*/

#include <stdlib.h>
#include <stddef.h>

/* static cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently stored */
static size_t cache_cap = 0;   /* allocated capacity */

/* Initial values for n = 1 … 10 */
static const seq_t init_vals[10] = {
    0ULL, 0ULL, 0ULL, 1ULL, 4ULL, 27ULL, 64ULL, 182ULL, 344ULL, 714ULL
};

seq_t a(idx_t n)
{
    /* Ensure the cache has enough capacity to store terms up to n */
    if (n > cache_cap) {
        seq_t *new_cache = (seq_t *)malloc((size_t)n * sizeof(seq_t));
        if (!new_cache) {
            /* Out‑of‑memory: return 0 (unlikely in the problem setting) */
            return (seq_t)0;
        }
        /* Copy the old values */
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        /* Free old cache */
        free(cache);
        cache = new_cache;
        cache_cap = (size_t)n;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        size_t idx = i + 1; /* 1‑based index */
        if (idx <= 10) {
            cache[i] = init_vals[idx - 1];
        } else {
            /* Apply the recurrence
               Use a two‑step accumulation to keep intermediate values
               unsigned and non‑negative. */
            seq_t pos =
                (seq_t)2 * cache[i - 1] +
                (seq_t)3 * cache[i - 2] +
                (seq_t)12 * cache[i - 5] +
                (seq_t)3 * cache[i - 8] +
                (seq_t)2 * cache[i - 9];
            seq_t neg =
                (seq_t)8 * cache[i - 3] +
                (seq_t)2 * cache[i - 4] +
                (seq_t)2 * cache[i - 6] +
                (seq_t)8 * cache[i - 7] +
                cache[i - 10];
            cache[i] = pos - neg;
        }
    }

    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}
