/*
 * A327739  Expansion of 1 / (1 - Sum_{i>=1} Sum_{j=1..i} x^(i*j)).
 * https://oeis.org/A327739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 61, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n) = Σ_{k=1..n} A038548(k) * a(n-k),  where
   A038548(k) = ⌈τ(k)/2⌉ = number of unordered factor pairs of k. */

/* This implementation memoizes the sequence in a static array.
   It recomputes up to the requested index on each call, and can
   handle arbitrary order of calls. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate and grow static arrays lazily so we can handle
       any n that is asked for, even if the calls are in arbitrary order. */
    static seq_t *a_arr = NULL;      /* a(n) values */
    static size_t a_cap = 0;         /* capacity of a_arr */
    static size_t a_len = 0;         /* number of precomputed values */

    static uint32_t *divcnt = NULL;  /* divisor counts τ(k) */
    static size_t divcnt_cap = 0;    /* capacity of divcnt */

    static uint32_t *b_arr = NULL;   /* A038548(k) values */
    static size_t b_cap = 0;         /* capacity of b_arr */

    static idx_t max_n = 0;          /* highest n for which we have computed a(n) */

    /* Ensure storage for indices up to n */
    if ((size_t)n + 1 > a_cap) {
        a_arr = (seq_t *)realloc(a_arr, ((size_t)n + 1) * sizeof(seq_t));
        a_cap = (size_t)n + 1;
    }
    if ((size_t)n + 1 > divcnt_cap) {
        divcnt = (uint32_t *)realloc(divcnt, ((size_t)n + 1) * sizeof(uint32_t));
        divcnt_cap = (size_t)n + 1;
    }
    if ((size_t)n + 1 > b_cap) {
        b_arr = (uint32_t *)realloc(b_arr, ((size_t)n + 1) * sizeof(uint32_t));
        b_cap = (size_t)n + 1;
    }

    /* Recompute values up to n if needed.  Use >= so that
       a(0) is computed the first time it is requested. */
    if (n >= max_n) {
        /* Compute divisor counts τ(k) for 0..n */
        for (size_t i = 0; i <= (size_t)n; ++i)
            divcnt[i] = 0;
        for (size_t i = 1; i <= (size_t)n; ++i) {
            for (size_t j = i; j <= (size_t)n; j += i)
                divcnt[j] += 1;
        }

        /* Compute A038548(k) = ceil(τ(k)/2) for k=0..n */
        b_arr[0] = 0;
        for (size_t i = 1; i <= (size_t)n; ++i)
            b_arr[i] = (divcnt[i] + 1) / 2;

        /* Compute sequence values up to n */
        if (a_len == 0) {          /* first call: set a(0)=1 */
            a_arr[0] = (seq_t)1;
            a_len = 1;
        }
        for (size_t m = a_len; m <= (size_t)n; ++m) {
            seq_t sum = 0;
            for (size_t k = 1; k <= m; ++k) {
                sum += ((seq_t)b_arr[k]) * a_arr[m - k];
            }
            a_arr[m] = sum;
        }
        a_len = (size_t)n + 1;
        max_n = n;
    }

    return a_arr[(size_t)n];
}
