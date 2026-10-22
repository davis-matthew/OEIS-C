/*
 * A337769  Smallest integer m such that the sum of the first m prime numbers
 *          is greater than n^2.
 * https://oeis.org/A337769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Min{m : Sum_{i=1..m} prime(i) > n^2}
 * Range:     u8 n <= 15, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* target = n^2, computed in seq_t to avoid overflow */
    seq_t target = (seq_t)n * (seq_t)n;

    /* static state for caching prime sums */
    static seq_t *prefix = NULL;          /* array of cumulative sums */
    static size_t prefix_cap   = 0;       /* allocated capacity of prefix */
    static size_t prefix_len   = 0;       /* current number of stored sums */
    static size_t next_k       = 1;       /* next prime index to fetch (1‑based) */
    static seq_t cumulative_sum = 0;      /* current sum of primes */

    /* initialise cache on first call */
    if (prefix == NULL) {
        prefix_cap = 16;
        prefix = (seq_t *)malloc(prefix_cap * sizeof(seq_t));
    }

    /* keep adding primes until the sum exceeds target */
    while (cumulative_sum <= target) {
        seq_t p = (seq_t)get_prime(next_k); /* get the next prime */
        cumulative_sum += p;

        /* grow the array if needed */
        if (prefix_len == prefix_cap) {
            prefix_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(prefix, prefix_cap * sizeof(seq_t));
            if (tmp) prefix = tmp; /* realloc handles failure silently for this use */
        }

        prefix[prefix_len++] = cumulative_sum;
        ++next_k;
    }

    /* next_k has been incremented past the prime that made the sum exceed target;
       the required m is the number of primes added so far, i.e. next_k‑1. */
    return (seq_t)(next_k - 1);
}
