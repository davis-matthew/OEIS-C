/*
 * A307529  G.f. A(x) satisfies: A(x) = (1 - x^2*A(x)^2)/(1 - x^2*A(x)^2 -
 *          x^3*A(x)^3).
 * https://oeis.org/A307529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 19, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   alloc_seq_array_local, free_seq_array_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) sequence A307529: 
   Generating function A(x) satisfies   A = (1 - x^2*A^2)/(1 - x^2*A^2 - x^3*A^3),
   with a(0)=1.  The implementation expands this functional equation
   coefficient‑wise using convolution sums (A^2, A^3, A^4). */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array_local.h"
#include "../utils/free_seq_array_local.h"

static seq_t *a_cache   = NULL;
static seq_t *c2_cache  = NULL;
static seq_t *c3_cache  = NULL;
static seq_t *c4_cache  = NULL;
static size_t cache_sz  = 0;   /* number of terms currently stored (>=1 after first init) */

/* Ensure that the cache holds terms up to index n (inclusive). */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_sz) return;          /* already computed */

    size_t new_sz = need;

    /* Allocate new arrays */
    seq_t *new_a  = alloc_seq_array_local(new_sz);
    seq_t *new_c2 = alloc_seq_array_local(new_sz);
    seq_t *new_c3 = alloc_seq_array_local(new_sz);
    seq_t *new_c4 = alloc_seq_array_local(new_sz);

    /* Copy existing data into the new arrays */
    for (size_t i = 0; i < cache_sz; ++i) {
        new_a[i]  = a_cache[i];
        new_c2[i] = c2_cache[i];
        new_c3[i] = c3_cache[i];
        new_c4[i] = c4_cache[i];
    }

    /* Compute new terms, starting from cache_sz up to n */
    for (size_t idx = cache_sz; idx < new_sz; ++idx) {
        idx_t i_idx = (idx_t)idx;
        seq_t a_val;

        if (i_idx == 0) {                     /* base case a(0) = 1 */
            a_val = (seq_t)1;
        } else if (i_idx <= 2) {              /* a(1) = a(2) = 0 */
            a_val = (seq_t)0;
        } else {
            a_val = new_c3[i_idx-2] - new_c2[i_idx-2] + new_c4[i_idx-3];
        }
        new_a[idx] = a_val;

        /* Compute C2[idx] = sum_{j=0..idx} a[j] * a[idx-j] */
        seq_t sum2 = (seq_t)0;
        for (size_t j = 0; j <= idx; ++j) {
            sum2 += new_a[j] * new_a[idx - j];
        }
        new_c2[idx] = sum2;

        /* Compute C3[idx] = sum_{j=0..idx} a[j] * C2[idx-j] */
        seq_t sum3 = (seq_t)0;
        for (size_t j = 0; j <= idx; ++j) {
            sum3 += new_a[j] * new_c2[idx - j];
        }
        new_c3[idx] = sum3;

        /* Compute C4[idx] = sum_{j=0..idx} a[j] * C3[idx-j] */
        seq_t sum4 = (seq_t)0;
        for (size_t j = 0; j <= idx; ++j) {
            sum4 += new_a[j] * new_c3[idx - j];
        }
        new_c4[idx] = sum4;
    }

    /* Free old arrays */
    if (a_cache)   free_seq_array_local(a_cache);
    if (c2_cache)  free_seq_array_local(c2_cache);
    if (c3_cache)  free_seq_array_local(c3_cache);
    if (c4_cache)  free_seq_array_local(c4_cache);

    /* Update pointers and size */
    a_cache   = new_a;
    c2_cache  = new_c2;
    c3_cache  = new_c3;
    c4_cache  = new_c4;
    cache_sz  = new_sz;
}

/* External linkage function required by the framework */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    return a_cache[(size_t)n];
}
