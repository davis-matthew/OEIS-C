/*
 * A349344  Dirichlet inverse of A109168, where A109168(n) = (n+A006519(n))/2,
 *          and A006519 is the highest power of 2 dividing n.
 * https://oeis.org/A349344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -∑_{d|n, d < n} A109168(n/d) * a(d) ; a(1) = 1
 * Range:     i8 n <= 252, i16 n <= 9779, i32 n <= 9779, i64 n <= 9779,
 *            i128 n <= 9779, big any n
 * Helpers:   A109168_from_idx, alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/A109168_from_idx.h"

/* cache for already computed values */
static seq_t      *a_cache        = NULL;
static size_t     a_cache_cap    = 0;
static idx_t      a_computed_max = 0;

/* ensure that the cache can hold indices up to 'need' */
static void ensure_a_cache(idx_t need)
{
    if ((size_t)need + 1 <= a_cache_cap) return;
    size_t new_cap = (size_t)need + 1;
    seq_t *new_ptr = alloc_seq_array(new_cap);
    if (!new_ptr) abort();                /* out of memory */
    /* copy existing values */
    for (size_t i = 0; i <= a_computed_max && i < a_cache_cap; ++i)
        new_ptr[i] = a_cache[i];
    free_seq_array(a_cache);
    a_cache        = new_ptr;
    a_cache_cap    = new_cap;
}

/* external function implementing the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined below offset */

    ensure_a_cache(n);

    if (n > a_computed_max) {
        /* initialise base case if needed */
        if (a_computed_max == 0) {
            a_cache[0] = 0;
            a_cache[1] = 1;
            a_computed_max = 1;
        }

        for (idx_t i = a_computed_max + 1; i <= n; ++i) {
            if (i == 1) {                /* already handled but keep for safety */
                a_cache[i] = 1;
                continue;
            }

            seq_t sum = 0;
            for (idx_t d = 1; d*d <= i; ++d) {
                if (i % d == 0) {
                    idx_t other = i / d;
                    if (d < i) {
                        sum += A109168_from_idx(other) * a_cache[d];
                    }
                    if (other != d && other < i) {
                        sum += A109168_from_idx(d) * a_cache[other];
                    }
                }
            }
            a_cache[i] = -sum;
        }
        a_computed_max = n;
    }
    return a_cache[n];
}
