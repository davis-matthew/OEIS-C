/*
 * A395251  a(n) gives positions k >= 1 where the equation b(b(k)+b(k-1)) = k
 *          fails, for b(k) = floor(k/sqrt(2) + 2*(sqrt(2)-1)).
 * https://oeis.org/A395251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to: a(n) = smallest k ≥ 1 with b(b(k)+b(k-1)) ≠ k
   where b(k) = ⌊ k/√2 + 2(√2–1) ⌋.
   The implementation computes the sequence on demand and caches
   the results for future calls.  No floating‑point arithmetic is
   used on seq_t values; double is used only for the Beatty
   computation which is accurate for all k needed here. */

#include <stdlib.h>
#include <math.h>

/* compute b(k) = floor( k/√2 + 2(√2‑1) ) using double precision.
   The added epsilon guarantees correct truncation even when
   k/√2 + const is extremely close to an integer. */
static inline uint64_t compute_b(idx_t k)
{
    /* initialise the constants just once */
    static double sqrt2 = 0.0;
    static double const_term = 0.0;
    if (sqrt2 == 0.0) {
        sqrt2 = sqrt(2.0);
        const_term = 2.0 * (sqrt2 - 1.0);
    }
    return (uint64_t)floor( (double)k / sqrt2 + const_term + 1e-12 );
}

seq_t a(idx_t n)
{
    /* cache of previously found terms */
    static seq_t *cache = NULL;      /* pointer to stored a(i) values    */
    static idx_t size  = 0;           /* number of terms currently stored */
    static idx_t cap   = 0;           /* current capacity of cache        */
    static idx_t next_k = 1;          /* next k to test                   */

    if (n == 0)
        return (seq_t)0;              /* undefined for offset 1, but safe */

    /* ensure the cache has enough space */
    if (cap == 0) {
        cap   = 64;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;          /* out of memory; return 0 as error */
    }

    /* generate terms until we have at least n values */
    while (size < n) {
        uint64_t bk  = compute_b(next_k);
        uint64_t bk1 = compute_b(next_k - 1);
        uint64_t t   = compute_b(bk + bk1);

        if (t != next_k) {
            if (size == cap) {        /* grow the cache when needed */
                idx_t new_cap = cap << 1;
                seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_ptr)
                    return (seq_t)0;  /* out of memory */
                cache = new_ptr;
                cap   = new_cap;
            }
            cache[size++] = (seq_t)next_k;   /* store the found term */
        }
        ++next_k;
    }

    return cache[n - 1];
}
