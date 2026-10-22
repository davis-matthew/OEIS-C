/*
 * A391712  Fibonacci numbers whose indices are even positive numbers with an
 *          even number of prime factors (counted with multiplicity).
 * https://oeis.org/A391712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   bigomega, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = Fibonacci(index) where “index” is the nth even positive integer
   that has an even number of prime factors counted with multiplicity
   (i.e. bigomega(index) is even).  The indices are generated on demand
   and cached so that calls may be in arbitrary order. */
#include <stdlib.h>
#include <stdint.h>

#include "../utils/fib.h"        /* seq_t fib(idx_t n); */
#include "../utils/bigomega.h"   /* seq_t bigomega(idx_t n); */

static idx_t *indices = NULL;   /* cached qualifying indices   */
static size_t indices_len = 0;  /* number of indices cached   */
static size_t indices_cap = 0;  /* current capacity           */

/* Grow the indices array to accommodate at least `needed` elements. */
static void grow_indices(size_t needed)
{
    if (needed <= indices_cap)
        return;
    size_t new_cap = indices_cap ? indices_cap * 2 : 4;
    while (new_cap < needed)
        new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(indices, new_cap * sizeof(idx_t));
    indices = tmp;
    indices_cap = new_cap;
}

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined but harmless */

    /* If we haven't generated enough indices yet, extend the cache. */
    if (indices_len < n) {
        idx_t cur = (indices_len == 0) ? 2 : indices[indices_len-1] + 2;
        while (indices_len < n) {
            seq_t omega = bigomega(cur);  /* number of prime factors (with multiplicity) */
            if ((omega & 1) == 0) {       /* even bigomega? */
                grow_indices(indices_len + 1);
                indices[indices_len++] = cur;
            }
            cur += 2;                     /* next even number */
        }
    }

    /* Return Fibonacci of the (n‑1)-th cached index. */
    return fib(indices[n - 1]);
}
