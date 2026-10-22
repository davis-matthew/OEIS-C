/*
 * A326242  Number of degree-n odd permutations of order dividing 12.
 * https://oeis.org/A326242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A326242: Number of degree‑n odd permutations of order dividing 12 */
#include <stdlib.h>

/* Static memoisation tables for odd and even permutations */
static seq_t *odd_cache = NULL;
static seq_t *even_cache = NULL;
static size_t cache_capacity = 0;
static idx_t computed_upto = 0;

/* Ensure the cache can hold indices up to n */
static void ensure_capacity(idx_t n)
{
    if (cache_capacity > n)
        return;

    size_t new_cap = cache_capacity ? cache_capacity * 2 : 64;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    odd_cache  = (seq_t *)realloc(odd_cache,  new_cap * sizeof(seq_t));
    even_cache = (seq_t *)realloc(even_cache, new_cap * sizeof(seq_t));

    for (size_t i = cache_capacity; i < new_cap; ++i) {
        odd_cache[i] = 0;
        even_cache[i] = 0;
    }
    cache_capacity = new_cap;

    /* Initial conditions for n = 0 */
    if (computed_upto == 0) {
        even_cache[0] = 1;   /* empty permutation : even */
        odd_cache[0]  = 0;   /* no odd permutation on 0 elements */
    }
}

/* Compute a(n) : number of odd permutations of degree n
 * whose cycle lengths all divide 12.  The result fits in seq_t
 * when it should; otherwise it is truncated to the capacity of seq_t. */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    if (computed_upto < n) {
        /* Divisors of 12 */
        const idx_t DIVS[6] = {1, 2, 3, 4, 6, 12};

        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            seq_t odd  = 0;
            seq_t even = 0;

            for (int d = 0; d < 6; ++d) {
                idx_t k = DIVS[d];
                if (k > i)
                    continue;

                /* Number of ways to choose a k‑cycle from i elements:
                   (i-1)! / (i-k)!  = product_{j=i-k+1}^{i-1} j              */
                seq_t coeff = 1;
                for (idx_t j = i - k + 1; j <= i - 1; ++j)
                    coeff *= j;

                if (((k - 1) & 1) == 0) {
                    /* Cycle is even (k odd) */
                    odd  += coeff * odd_cache[i - k];
                    even += coeff * even_cache[i - k];
                } else {
                    /* Cycle is odd (k even) */
                    odd  += coeff * even_cache[i - k];
                    even += coeff * odd_cache[i - k];
                }
            }

            odd_cache[i]  = odd;
            even_cache[i] = even;
        }
        computed_upto = n;
    }

    return odd_cache[n];
}
