/*
 * A285811  Primes equal to a centered heptagonal number plus 1.
 * https://oeis.org/A285811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 23, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes p such that p = (7·m² – 7·m + 4)/2 for some integer m ≥ 1 */
/* The function searches m sequentially and tests each candidate for primality. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* Cached primes that we've found so far */
    static seq_t *cache = NULL;
    static size_t size  = 0;   /* number of primes stored */
    static size_t cap   = 0;   /* capacity of the cache array */
    static idx_t  last_m = 0;  /* last m value examined */

    if (n == 0) {
        /* The OEIS offset is 1; a(0) is undefined; return 0 for safety. */
        return 0;
    }

    /* Keep generating candidates until we have at least n primes. */
    while (size < (size_t)n) {
        /* Next m value to test. */
        idx_t m = ++last_m;
        seq_t mm = (seq_t)m;

        /* Compute p = (7*m² – 7*m + 4)/2. */
        seq_t val = (((seq_t)7 * mm * mm) - ((seq_t)7 * mm) + (seq_t)4) / 2;

        /* If val is prime, store it in the cache. */
        if (is_prime_big(val)) {
            append_candidate(&cache, &size, &cap, val);
        }
    }

    return cache[n - 1];
}
