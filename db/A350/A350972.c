/*
 * A350972  E.g.f. = tan(x).
 * https://oeis.org/A350972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 24, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1; for n>=2, a(2k)=0,
   a(2k+1)=∑_{i=0}^{k-1} C(2k,2i+1)·a(2i+1)·a(2(k-1-i)+1).   */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *tangent_cache = NULL;
static size_t tangent_cap   = 0;
static size_t tangent_used  = 0;

/* ensure tangent_cache has at least 'needed' entries */
static void ensure_tangent_cap(size_t needed)
{
    if (tangent_cap >= needed) return;
    size_t new_cap = tangent_cap ? tangent_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    tangent_cache = (seq_t *)realloc(tangent_cache, new_cap * sizeof(seq_t));
    tangent_cap = new_cap;
}

/* compute tangent numbers up to index m = (n-1)/2 */
static void compute_tangent_upto(idx_t m)
{
    if ((size_t)m < tangent_used) return;
    ensure_tangent_cap((size_t)m + 1);

    /* initialise first tangent number T_1 = 1 */
    if (tangent_used == 0) {
        tangent_cache[0] = (seq_t)1;
        tangent_used = 1;
    }

    for (size_t i = tangent_used; i <= (size_t)m; ++i) {
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k < i; ++k) {
            seq_t bin = binomial((idx_t)(2*i), (idx_t)(2*k+1));
            seq_t prod = bin * tangent_cache[k] * tangent_cache[i-1-k];
            sum += prod;
        }
        tangent_cache[i] = sum;
    }
    tangent_used = (size_t)m + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if ((n & (idx_t)1) == 0) return (seq_t)0;   /* even indices */

    idx_t m = (n - (idx_t)1) / (idx_t)2;
    compute_tangent_upto(m);
    return tangent_cache[(size_t)m];
}
