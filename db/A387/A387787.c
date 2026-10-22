/*
 * A387787  Positive integers k for which the Collatz 3x+1 dropping value
 *          (first value < k) is prime.
 * https://oeis.org/A387787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A387787:
   a(n) = nth positive integer k such that the first Collatz value
   below k is prime.
   This implementation performs an on‑demand search, caching the
   found terms.  A simple trial division primality test is used.
*/

#include <stdbool.h>
#include <stdlib.h>

/* Simple primality test for seq_t values. */
static bool is_prime_seq(seq_t n)
{
    if (n < (seq_t)2)
        return false;
    if (n == (seq_t)2)
        return true;
    if ((n & (seq_t)1) == 0)
        return false;          /* even numbers >2 are composite */

    for (seq_t d = (seq_t)3; d <= n / d; d += (seq_t)2) {
        if (n % d == 0)
            return false;
    }
    return true;
}

/* Determines whether the first Collatz value < k is prime. */
static bool drop_value_is_prime(seq_t k)
{
    if (k <= (seq_t)1)
        return false;

    seq_t x = k;
    while (1) {
        if ((x & (seq_t)1))
            x = (seq_t)3 * x + (seq_t)1;
        else
            x >>= 1;

        if (x < k)            /* first value below k reached */
            return is_prime_seq(x);
    }
}

/* Return the nth element of the sequence (1-indexed). */
seq_t a(idx_t n)
{
    /* Cache of previously found terms. */
    static seq_t *cache      = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_k      = (seq_t)0;  /* largest k already tested */

    while (cache_len < n) {
        ++last_k;
        if (!drop_value_is_prime(last_k))
            continue;          /* k is not part of the sequence */

        /* Grow cache if needed. */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = last_k;
    }
    return cache[n - 1];
}
