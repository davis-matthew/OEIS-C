/*
 * A262815  Number of ordered ways to write n as x^3 + y*(y+1)/2 +
 *          z*(3*z+1)/2, where x, y and z are nonnegative integers.
 * https://oeis.org/A262815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A262815
    a(n) = number of ordered triples (x,y,z) of non‑negative integers
    satisfying  n = x^3 + y(y+1)/2 + z(3z+1)/2
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper for allocating an array of seq_t */
static seq_t* alloc_array(idx_t len)
{
#ifdef __cplusplus
    return new seq_t[len]();          /* value‑initialize each element */
#else
    return (seq_t*)calloc((size_t)len, sizeof(seq_t));
#endif
}

/* Helper for freeing an array of seq_t */
static void free_array(seq_t* p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Cached tables */
static seq_t* sums_counts = NULL;   /* sums_counts[sum] = # of (y,z) with
                                      T_y + P_z = sum */
static size_t sums_cap   = 0;

static seq_t* a_cache    = NULL;    /* a_cache[n] = answer for n */
static size_t a_cap      = 0;

/* Extend cached tables to include all n up to new_n (inclusive). */
static void extend_to(idx_t new_n)
{
    size_t new_cap = (size_t)(new_n + 1);

    /* Build table of (y,z) sums up to new_n */
    seq_t* new_sums = alloc_array(new_cap);
    for (idx_t y = 0; ; ++y) {
        seq_t tri = (seq_t)y * (y + 1) / 2;
        if (tri > new_n) break;
        for (idx_t z = 0; ; ++z) {
            seq_t pent = (seq_t)z * (3 * z + 1) / 2;
            seq_t sum  = tri + pent;
            if (sum > new_n) break;
            ++new_sums[(size_t)sum];
        }
    }
    seq_t* old_sums = sums_counts;
    sums_counts = new_sums;
    sums_cap = new_cap;
    free_array(old_sums);

    /* Compute a(n) values up to new_n using the prepared sums table. */
    seq_t* new_cache = alloc_array(new_cap);
    for (idx_t m = 0; m <= new_n; ++m) {
        seq_t total = 0;
        for (idx_t x = 0; ; ++x) {
            seq_t cube = (seq_t)x * x * x;
            if (cube > m) break;
            total += sums_counts[(size_t)(m - cube)];
        }
        new_cache[(size_t)m] = total;
    }
    seq_t* old_cache = a_cache;
    a_cache = new_cache;
    a_cap = new_cap;
    free_array(old_cache);
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if ((size_t)(n + 1) > a_cap)   /* extend cache if necessary */
        extend_to(n);
    return a_cache[(size_t)n];
}
