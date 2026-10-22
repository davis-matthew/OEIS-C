/*
 * A309389  a(n) is the smallest positive divisor not yet in the sequence of
 *          11*A000217(n-1); n >= 1.
 * https://oeis.org/A309389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the smallest positive divisor of 11*T(n-1) (T(n-1) = (n-1)*n/2)
   that has not already appeared in a(1)..a(n-1).
   The function is memoised; it works for any order of calls. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/grow_seq_array.h"   /* ensures dynamic array growth */

static seq_t *seq_values = NULL;  /* array of computed a(k) */
static size_t seq_cap   = 0;      /* current capacity of seq_values   */
static idx_t   seq_len   = 0;      /* number of values already stored */

/* Linear search: true if x has been seen before */
static bool seen(seq_t x)
{
    for (idx_t i = 0; i < seq_len; ++i)
        if (seq_values[i] == x)
            return true;
    return false;
}

/* Find a(n) (n >= 1) */
seq_t a(idx_t n)
{
    /* Grow the cache up to index n */
    if (n == 0)
        return 0;   /* undefined for n<1, but guard against underflow */

    /* Extend the cache if needed */
    if (seq_len < n) {
        /* Ensure sufficient capacity for all indices up to n */
        grow_seq_array(&seq_values, &seq_cap, (size_t)n);
        /* Compute missing terms */
        for (idx_t k = seq_len + 1; k <= n; ++k) {
            seq_t value;
            if (k == 1) {
                /* 11 * T(0) = 0 => every positive integer is a divisor */
                value = 1;
            } else {
                /* Compute m = 11 * ((k-1)*k/2) using seq_t arithmetic */
                seq_t prod = (seq_t)(k - 1);
                prod *= (seq_t)k;          /* (k-1)*k */
                prod /= (seq_t)2;          /* /2 */
                seq_t m = (seq_t)11 * prod;/* 11*T(k-1) */
                /* Find smallest divisor not yet seen */
                for (seq_t d = 1; d <= m; ++d) {
                    if (m % d == 0 && !seen(d)) {
                        value = d;
                        break;
                    }
                }
            }
            /* Append to cache */
            seq_values[seq_len++] = value;
        }
    }
    /* Return the cached value */
    return seq_values[n - 1];
}
