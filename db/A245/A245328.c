/*
 * A245328  Denominators in recursive bijection from positive integers to
 *          positive rationals (the bijection is f(1) = 1, f(2n) = 1/(f(n)+1),
 *          f(2n+1) = f(n)+1).
 * https://oeis.org/A245328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 4095, u16 n <= 9500, u32 n <= 9500, u64 n <= 9500,
 *            u128 n <= 9500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of the recursive bijection
 *   f(1)       = 1
 *   f(2n)      = 1 / ( f(n) + 1 )
 *   f(2n+1)    =   f(n) + 1
 *
 * If f(n) = p/q in lowest terms then
 *   denom[1] = 1
 *   denom[2n]     = p[n] + q[n]
 *   denom[2n+1]   = q[n]
 *
 * The function a(idx_t n) returns denom(n). */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Ptrs to cached numerator and denominator arrays.
 * The arrays are grown lazily when a larger index is requested.
 */
static seq_t *num   = NULL;
static seq_t *den   = NULL;
static idx_t  cache_cap = 0;

/* Ensure that the arrays can hold at least indices 0 … n.
 * New elements are zero‑initialized, and the base case
 * (n = 1, value 1/1) is installed if not already present. */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap <= n)
        new_cap = n + 1;

    /* Allocate new larger arrays */
    seq_t *tmp_num = (seq_t*)malloc(new_cap * sizeof(seq_t));
    seq_t *tmp_den = (seq_t*)malloc(new_cap * sizeof(seq_t));

    if (!tmp_num || !tmp_den) {
        /* Allocation failed – abort on a build that expects a valid result */
        abort();
    }

    if (num) {
        memcpy(tmp_num, num, cache_cap * sizeof(seq_t));
        memcpy(tmp_den, den, cache_cap * sizeof(seq_t));
        free(num);
        free(den);
    }

    /* Zero‑initialize the new slots. */
    for (idx_t i = cache_cap; i < new_cap; ++i) {
        tmp_num[i] = (seq_t)0;
        tmp_den[i] = (seq_t)0;
    }

    num   = tmp_num;
    den   = tmp_den;
    cache_cap = new_cap;

    /* Base case: f(1) = 1/1 */
    if (num[1] == (seq_t)0) {
        num[1] = (seq_t)1;
        den[1] = (seq_t)1;
    }
}

/* Recursive computation of numerator and denominator for index n.
 * The recursion is tail‑recursive over the binary representation
 * of n and uses only O(log n) stack depth. */
static void compute_recursive(idx_t n)
{
    if (den[n] != (seq_t)0)            /* already computed */
        return;

    idx_t m = n >> 1;                   /* floor(n/2) */
    compute_recursive(m);

    seq_t a = num[m];
    seq_t b = den[m];

    if ((n & 1U) == 0) {                /* n even : f(2m) = b / (a + b) */
        num[n] = b;
        den[n] = a + b;
    } else {                            /* n odd  : f(2m+1) = (a + b) / b */
        num[n] = a + b;
        den[n] = b;
    }
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined, but safe fallback */

    ensure_capacity(n);
    compute_recursive(n);
    return den[n];
}
