/*
 * A267826  Numbers not of the form w^3 + 2*x^3 + 3*y^3 + 4*z^3, where w, x, y
 *          and z are nonnegative integers.
 * https://oeis.org/A267826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 122, u32 n <= 122, u64 n <= 122,
 *            u128 n <= 122, big any n
 * Helpers:   int_cube_root, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A267826: Numbers not of the form w^3 + 2*x^3 + 3*y^3 + 4*z^3
 * This implementation builds a table of all representable sums up to a
 * dynamically increasing bound and then returns the n‑th missing
 * (non‑representable) nonnegative integer.  The table is rebuilt only
 * when a larger bound is needed.
 */

#include <stdlib.h>   /* malloc, calloc, free, abort */
#include <stddef.h>   /* size_t */

#include "../utils/pow_int.h"          /* seq_t pow_int(seq_t base, idx_t exp) */
#include "../utils/int_cube_root.h"   /* idx_t int_cube_root(idx_t n) */

static uint8_t *repr = NULL;   /* boolean table: 0 = not representable */
static idx_t L_current = 0;     /* highest value for which repr is built */

/* Helper to allocate an array of seq_t, works in both C and C++ */
static seq_t *alloc_seq_array(size_t sz)
{
#ifdef __cplusplus
    return new seq_t[sz];
#else
    return (seq_t *)malloc(sz * sizeof(seq_t));
#endif
}

/* Helper to free an array of seq_t, works in both C and C++ */
static void free_seq_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Rebuild the representation table up to 'limit' (inclusive). */
static void rebuild_table(idx_t limit)
{
    /* Free old table */
    if (repr) {
        free(repr);
        repr = NULL;
    }

    /* Allocate new table initialized to zero */
    repr = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (!repr) abort();

    /* Compute maximal variable values for given limit */
    idx_t w_max = int_cube_root(limit);
    idx_t x_max = int_cube_root(limit / 2);
    idx_t y_max = int_cube_root(limit / 3);
    idx_t z_max = int_cube_root(limit / 4);

    /* Pre‑compute weighted cubes */
    seq_t *cube_w   = alloc_seq_array((size_t)(w_max + 1));
    seq_t *arr2x    = alloc_seq_array((size_t)(x_max + 1));
    seq_t *arr3y    = alloc_seq_array((size_t)(y_max + 1));
    seq_t *arr4z    = alloc_seq_array((size_t)(z_max + 1));
    if (!cube_w || !arr2x || !arr3y || !arr4z) abort();

    for (idx_t i = 0; i <= w_max; ++i)
        cube_w[i] = pow_int((seq_t)i, 3);
    for (idx_t i = 0; i <= x_max; ++i)
        arr2x[i] = 2 * pow_int((seq_t)i, 3);
    for (idx_t i = 0; i <= y_max; ++i)
        arr3y[i] = 3 * pow_int((seq_t)i, 3);
    for (idx_t i = 0; i <= z_max; ++i)
        arr4z[i] = 4 * pow_int((seq_t)i, 3);

    /* Enumerate all representable sums up to 'limit' */
    seq_t lim_seq = (seq_t)limit;
    for (idx_t w = 0; w <= w_max; ++w) {
        seq_t val_w = cube_w[w];
        for (idx_t x = 0; x <= x_max; ++x) {
            seq_t val_wx = val_w + arr2x[x];
            if (val_wx > lim_seq) break;
            for (idx_t y = 0; y <= y_max; ++y) {
                seq_t val_wxy = val_wx + arr3y[y];
                if (val_wxy > lim_seq) break;
                for (idx_t z = 0; z <= z_max; ++z) {
                    seq_t total = val_wxy + arr4z[z];
                    if (total > lim_seq) break;
                    repr[(idx_t)total] = 1;
                }
            }
        }
    }

    /* Clean up temporary arrays */
    free_seq_array(cube_w);
    free_seq_array(arr2x);
    free_seq_array(arr3y);
    free_seq_array(arr4z);

    L_current = limit;
}

/* Return the n-th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0 */

    /* Rough upper bound: density of representable numbers ≈ 99.7%,
       roughly one missing number per 350 integers.  Increase slightly
       to be safe. */
    idx_t target = 400ULL * n + 500;
    if (target < 500) target = 500;

    /* Build table if needed */
    if (L_current < target) rebuild_table(target);

    /* Find n-th missing number */
    idx_t count = 0;
    for (idx_t i = 0; i <= L_current; ++i) {
        if (!repr[i]) {
            ++count;
            if (count == n) return (seq_t)i;
        }
    }

    /* If not found, increase limit and try again */
    idx_t new_limit = L_current ? 2 * L_current : 1;
    while (1) {
        if (new_limit < target) new_limit = target;
        rebuild_table(new_limit);
        count = 0;
        for (idx_t i = 0; i <= new_limit; ++i) {
            if (!repr[i]) {
                ++count;
                if (count == n) return (seq_t)i;
            }
        }
        new_limit *= 2;
    }
}
