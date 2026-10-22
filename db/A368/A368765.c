/*
 * A368765  a(n) = n! * (1 + Sum_{k=0..n} (-1)^k * k / k!).
 * https://oeis.org/A368765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (-1)^n*n, a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;     /* array of pre‑computed values   */
static size_t a_cache_len = 0;    /* number of entries currently in a_cache */

/*  Returns the n-th term of OEIS sequence A368765.
    The function may be called in any order; previously computed
    values are memoised.                           */
seq_t a(idx_t n) {
    /* Ensure the base case a(0) = 1 is stored. */
    if (n == 0) {
        if (a_cache_len == 0) {
            a_cache = (seq_t *)malloc(sizeof(seq_t));
            a_cache_len = 1;
            a_cache[0] = (seq_t)1;
        }
        return a_cache[0];
    }

    /* Extend the cache if the requested index is beyond what we have. */
    if ((size_t)n >= a_cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
        a_cache = tmp;

        if (a_cache_len == 0) {     /* first time: initialise a(0) */
            a_cache[0] = (seq_t)1;
            a_cache_len = 1;
        }

        /* Compute terms from the next unused index up to n. */
        for (size_t i = a_cache_len; i <= n; ++i) {
            /* a(i) = i * a(i-1) + (-1)^i * i */
            seq_t prod = a_cache[i - 1] * (seq_t)i;
            if (i & 1) {            /* i odd: subtract i  */
                prod -= (seq_t)i;
            } else {                /* i even: add    i */
                prod += (seq_t)i;
            }
            a_cache[i] = prod;
        }
        a_cache_len = new_len;
    }

    return a_cache[(size_t)n];
}
