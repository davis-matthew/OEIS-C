/*
 * A336969  a(n) = n! * [x^n] 1 / (exp(n*x) - x).
 * https://oeis.org/A336969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..n} (-n*(n-k+1))^k / k!
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 13, i128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

static void seq_free_array(seq_t* p)          { mem_free(p); }

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0) return 1;

    /* Precompute prod[k] = product_{i=k+1}^{n} i  (== n! / k!) */
    size_t len = (size_t)n + 1;
    seq_t *prod = seq_alloc_new_array(len);
    prod[n] = 1;                       /* empty product for k = n */
    for (idx_t k = n; k > 0; --k) {    /* build backwards */
        prod[k-1] = prod[k] * (seq_t)k;
    }

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t base = -(seq_t)n * ((seq_t)n - (seq_t)k + 1);
        seq_t pow_val = ipow_idx(base, k);
        seq_t term = pow_val * prod[k];
        sum += term;
    }

    seq_free_array(prod);
    return sum;
}
