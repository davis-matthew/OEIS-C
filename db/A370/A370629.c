/*
 * A370629  Lexicographically earliest sequence of distinct positive integers
 *          such that for any n > 0, the Zeckendorf expansions of n and a(n)
 *          have exactly one common term.
 * https://oeis.org/A370629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_single_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: greedy construction of the self‑inverse permutation
   A370629.  For each integer i (starting from 1) we assign a(i) as
   follows:
        • If i is a Fibonacci number, a(i)=i.
        • Otherwise find the smallest integer j>i that is not yet used
          and such that the Zeckendorf expansions of i and j share
          exactly one Fibonacci term.  Set a(i)=j and a(j)=i.
   This produces the lexicographically earliest permutation of the
   positive integers satisfying the Zeckendorf‑intersection condition.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_single_bit.h"

/* ------------------------------------------------------------------ */
/* Pre‑compute Fibonacci numbers 1,2,3,5,… up to 64‑bit overflow.     */
static unsigned long long fibs[90];
static int fib_len = 0;
static bool fibs_initialized = false;

static void init_fibs(void)
{
    if (fibs_initialized) return;
    fibs[0] = 1;
    fibs[1] = 2;
    fib_len = 2;
    while (1) {
        unsigned long long next = fibs[fib_len - 1] + fibs[fib_len - 2];
        if (next < fibs[fib_len - 1]) break;          /* overflow */
        fibs[fib_len++] = next;
        if (fib_len >= (int)(sizeof(fibs)/sizeof(fibs[0]))) break;
    }
    fibs_initialized = true;
}

/* Zeckendorf mask of a positive integer n:
   bit i is set iff Fibonacci fibs[i] is used in the expansion of n. */
static uint64_t zeck_mask(idx_t n)
{
    uint64_t m = 0;
    for (int i = fib_len - 1; i >= 0; --i) {
        if (fibs[i] <= n) {
            n -= fibs[i];
            m |= ((uint64_t)1 << i);
        }
    }
    return m;
}

/* ------------------------------------------------------------------ */
/* Static memoisation tables */
static seq_t *perm      = NULL;      /* perm[i] = a(i) (0 if unused) */
static uint64_t *mask_arr = NULL;    /* Zeckendorf mask of index i  */
static idx_t   perm_cap   = 0;        /* current capacity of arrays */

/* Ensure storage large enough to hold indices up to 'up_to'. */
static void ensure_capacity(idx_t up_to)
{
    if (up_to < perm_cap) return;
    idx_t old_cap = perm_cap;
    idx_t new_cap = perm_cap ? (perm_cap << 1) : 4;
    while (new_cap <= up_to) new_cap <<= 1;

    perm     = (seq_t *)realloc(perm,     new_cap * sizeof(seq_t));
    mask_arr = (uint64_t *)realloc(mask_arr, new_cap * sizeof(uint64_t));
    if (!perm || !mask_arr) exit(EXIT_FAILURE);

    for (idx_t i = old_cap; i < new_cap; ++i) {
        perm[i] = 0;
        if (i > 0) mask_arr[i] = zeck_mask(i);
    }
    /* mask_arr[0] remains 0 (unused) */
    perm_cap = new_cap;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;       /* undefined for n < 1 */

    init_fibs();
    ensure_capacity(n);

    /* Process all indices up to n in ascending order.  The permutation
       is built lazily; once an index is processed it is never changed. */
    for (idx_t i = 1; i <= n; ++i) {
        if (perm[i] != 0) continue;          /* already assigned */

        if (has_single_bit(mask_arr[i])) {    /* Fibonacci number */
            perm[i] = (seq_t)i;
            continue;
        }

        /* Find smallest unused j > i such that i and j have exactly one
           Zeckendorf term in common. */
        for (idx_t j = i + 1; ; ++j) {
            ensure_capacity(j);
            if (perm[j] != 0) continue;      /* already used */

            uint64_t common = mask_arr[i] & mask_arr[j];
            if (common && has_single_bit(common)) {
                perm[i] = (seq_t)j;
                perm[j] = (seq_t)i;
                break;
            }
        }
    }

    return perm[n];
}
