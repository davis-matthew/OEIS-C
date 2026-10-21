/*
 * A261359  Pentatope of coefficients in expansion of (1 + x + 2*y + 2*z)^n.
 * https://oeis.org/A261359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, walk, less
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 125, u32 n <= 125, u64 n <= 125,
 *            u128 n <= 125, big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = value of the nth element in the flattened list of coefficients
 *              T(m,i,j,k) = 2^j * binomial(m,i) * binomial(i,j) * binomial(j,k)
 *              taken for all m >= 0, i = 0..m, j = 0..i, k = 0..j and
 *              concatenated in increasing (m,i,j,k) order.
 *          That is, we list all terms of the expansion of (1 + x + 2y + 2z)^m
 *          slice by slice (m = 0,1,2,…) and inside a slice in lexicographic
 *          order of (i,j,k).  
 *  Memoize the generated terms so each call to a(idx_t n) runs in
 *  O(1) after the first few calls.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* utilities */
#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/pow2.h"    /* seq_t pow2(idx_t exp); */

static seq_t *cache        = NULL;   /* array of generated terms   */
static idx_t  cache_cap    = 0;      /* capacity of cache         */
static idx_t  cache_len    = 0;      /* number of terms stored    */
static idx_t  next_slice   = 0;      /* next slice number to generate */

/* ensure capacity for at least needed elements */
static void
ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) {
        /* out of memory – terminate the program safely */
        abort();
    }
    cache = new_buf;
    cache_cap = new_cap;
}

/* append a term to the cache */
static void
cache_append(seq_t val)
{
    ensure_capacity(cache_len + 1);
    cache[cache_len++] = val;
}

/* generate all coefficients for slice 'n', in lexicographic order (i,j,k) */
static void
generate_slice(idx_t n)
{
    for (idx_t i = 0; i <= n; ++i) {
        seq_t binom_n_i = binom(n, i);
        for (idx_t j = 0; j <= i; ++j) {
            seq_t binom_i_j   = binom(i, j);
            seq_t pow2_j      = pow2(j);
            for (idx_t k = 0; k <= j; ++k) {
                seq_t binom_j_k = binom(j, k);
                /* T(n,i,j,k) = 2^j * C(n,i) * C(i,j) * C(j,k) */
                seq_t val = pow2_j * binom_n_i * binom_i_j * binom_j_k;
                cache_append(val);
            }
        }
    }
}

/* public function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* generate slices until we have at least n+1 terms */
    while (cache_len <= n) {
        generate_slice(next_slice);
        ++next_slice;
    }
    return cache[n];
}
