/*
 * A249837  a(n+1) is the next smallest nontrivial cube beginning with a(n),
 *          initial term is 9.
 * https://oeis.org/A249837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   floor_cube_root, pow10, seq_alloc, seq_mul, seq_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n+1) is the next smallest nontrivial cube beginning with a(n)
   (nontrivial means the cube root is not divisible by 10) */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow10.h"           /* seq_t pow10(idx_t e)          */
#include "../utils/seq_mul.h"         /* seq_t seq_mul(seq_t, seq_t)   */
#include "../utils/seq_pow.h"         /* seq_t seq_pow(seq_t, idx_t)   */
#include "../utils/floor_cube_root.h" /* seq_t floor_cube_root(seq_t)  */
#include "../utils/seq_alloc.h"       /* seq_t* seq_alloc(size_t)      */
#include "../utils/seq_realloc.h"     /* seq_t* seq_realloc(seq_t*,size_t,size_t) */

/* Return the smallest nontrivial cube whose decimal representation starts
   with the digits of `prev`.  The cube must be strictly larger than
   `prev`. */
static inline seq_t
next_cube_with_prefix(seq_t prev)
{
    /* We search over leading‑digit blocks 10^k.  For each block we
       find the smallest cube ≥ prev·10^k that lies in
       [prev·10^k , (prev+1)·10^k).  We skip cubes whose root is
       divisible by 10. */
    seq_t pow10_t = (seq_t)1;
    for (;;) {
        seq_t L = seq_mul(prev, pow10_t);                    /* prev·10^k */
        seq_t U = seq_mul(prev + (seq_t)1, pow10_t);         /* (prev+1)·10^k */

        /* Smallest integer `x` with x^3 ≥ L. */
        seq_t x = floor_cube_root(L);
        seq_t cube = seq_pow(x, (idx_t)3);
        while (cube < L) {
            ++x;
            cube = seq_pow(x, (idx_t)3);
        }

        /* Skip any cube whose root is divisible by 10. */
        while (cube < U && (x % (seq_t)10) == (seq_t)0) {
            ++x;
            cube = seq_pow(x, (idx_t)3);
        }

        /* If a valid cube is found, return it. */
        if (cube < U && cube > prev)
            return cube;

        /* Try the next digit block. */
        pow10_t = seq_mul(pow10_t, (seq_t)10);
    }
}

/* The required sequence function. */
seq_t
a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined for n<1 */

    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t computed = 0;            /* highest index already computed */

    /* Initial allocation. */
    if (cache == NULL) {
        capacity = 8;
        cache = seq_alloc(capacity);
        computed = 0;
    }

    /* Grow cache if necessary. */
    if ((idx_t)capacity < n) {
        size_t new_cap = capacity * 2;
        if ((idx_t)new_cap < n) new_cap = (size_t)n + 8;
        cache = seq_realloc(cache, capacity, new_cap);
        capacity = new_cap;
    }

    /* Compute any missing terms. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == (idx_t)1) {
            cache[i - 1] = (seq_t)9;
        } else {
            cache[i - 1] = next_cube_with_prefix(cache[i - 2]);
        }
    }
    computed = n;

    return cache[n - 1];
}
