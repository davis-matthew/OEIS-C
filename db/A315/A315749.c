/*
 * A315749  Coordination sequence Gal.5.295.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 6 a(2) = 12 a(3) = 18 a(4) = 23 a(5) = 29 a(6) =
 *            35 a(7) = 40 a(8) = 46 a(9) = 52 a(10) = 58 For n > 10: a(n) =
 *            2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) - 2*a(n-6)
 *            + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10)
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoization with a static cache so that
   the function can be called in any order and the recurrence is
   evaluated only as far as needed.  It works for any seq_t defined
   by the build system (uint8_t … uint128_t or an arbitrary‑precision
   class).  The recurrence is computed using only integer arithmetic
   with the same type as the sequence values.  */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init[11] = {
    1, 6, 12, 18, 23, 29, 35, 40, 46, 52, 58
};

static seq_t *cache = NULL;        /* dynamic array holding computed values */
static size_t cache_cap = 0;       /* current capacity of the array     */
static idx_t max_comp = 0;         /* largest index for which a value is stored */

seq_t a(idx_t n)
{
    /* Return known values for the first 11 indices. */
    if (n < 11) return init[n];

    /* Resize cache if necessary. */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {              /* allocation failed – abort */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Ensure that the initial 11 values are present in the cache. */
    if (max_comp < 10) {
        for (idx_t i = 0; i <= 10; ++i)
            cache[i] = init[i];
        max_comp = 10;
    }

    /* Compute missing values up to n using the recurrence. */
    for (idx_t k = max_comp + 1; k <= n; ++k) {
        /* Positive part: 2*a(k-1) + 2*a(k-3) + ... + 2*a(k-9) */
        seq_t pos_sum = 0;
        for (int i = 1; i <= 9; i += 2)
            pos_sum += cache[k - i] << 1;   /* <<1 == *2 for unsigned */

        /* Negative part: 2*a(k-2) + 2*a(k-4) + ... + 2*a(k-8) + a(k-10) */
        seq_t neg_sum = 0;
        for (int i = 2; i <= 8; i += 2)
            neg_sum += cache[k - i] << 1;
        neg_sum += cache[k - 10];

        cache[k] = pos_sum - neg_sum;       /* guaranteed non‑negative */
    }

    max_comp = n;
    return cache[n];
}
