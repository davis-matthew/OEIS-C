/*
 * A326894  a(1) = 1; thereafter, a(n) is the reversal of the next prime after
 *          a(n - 1) in base 3 if n is prime, and the reversal of the next
 *          composite after a(n - 1) in base 3 if n is composite.
 * https://oeis.org/A326894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9813, u16 n <= 9813, u32 n <= 9813, u64 n <= 9813,
 *            u128 n <= 9813, big any n
 * Helpers:   is_prime, is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A326894:
   a(1) = 1;
   for n ≥ 2,
       if n is prime      a(n) = reverse_base3( next_prime_after( a(n-1) ) )
       if n is composite a(n) = reverse_base3( next_composite_after( a(n-1) ) )
   where reverse_base3 reverses the base‑3 digits of its argument.   */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* utility headers provided by the build system */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/is_prime.h"      /* bool is_prime(idx_t x); */
#include "../utils/seq_alloc.h"    /* seq_t* seq_alloc(size_t); */
#include "../utils/seq_realloc.h"  /* seq_t* seq_realloc(seq_t*,size_t,size_t); */

static seq_t *cache = NULL;     /* 0‑based cache of computed a(n) values */
static size_t cap = 0;          /* current capacity of cache (number of slots) */
static idx_t max_computed = 0;  /* largest n that has been computed (1‑based) */

/* Grow the cache so that it holds at least 'required' elements.  */
static void ensure_capacity(size_t required)
{
    if (required <= cap) return;
    size_t newcap = required;
    if (!cache) {
        cache = seq_alloc(newcap);
    } else {
        cache = seq_realloc(cache, cap, newcap);
    }
    cap = newcap;
}

/* Return the smallest prime strictly greater than x (x is seq_t).  */
static seq_t next_prime_after(seq_t x)
{
    seq_t cand = x + 1;
    while (!is_prime_big(cand)) {
        cand = cand + 1;
    }
    return cand;
}

/* Return the smallest composite strictly greater than x (x is seq_t).  */
static seq_t next_composite_after(seq_t x)
{
    seq_t cand = x + 1;
    while (cand <= 1 || is_prime_big(cand)) {
        cand = cand + 1;
    }
    return cand;
}

/* Reverse the base‑3 digits of n and return the resulting number.  */
static seq_t reverse_base3(seq_t n)
{
    if (n == (seq_t)0) return (seq_t)0;
    /* Collect base‑3 digits (least significant first). */
    uint8_t digits[256];
    size_t len = 0;
    seq_t tmp = n;
    while (tmp > (seq_t)0) {
        digits[len++] = (uint8_t)(tmp % (seq_t)3);
        tmp = tmp / (seq_t)3;
    }
    /* Reconstruct number using the digits in their collected order
       which corresponds to the reversed digit string. */
    seq_t rev = (seq_t)0;
    for (size_t i = 0; i < len; ++i) {
        rev = rev * (seq_t)3 + (seq_t)digits[i];
    }
    return rev;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    /* Ensure cache can hold value at index n-1. */
    ensure_capacity((size_t)n);

    /* Compute values up to n if necessary. */
    if (max_computed < n) {
        if (max_computed == 0) {          /* initialise a(1) = 1 */
            cache[0] = (seq_t)1;
            max_computed = 1;
        }
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t prev = cache[i - 2];    /* a(i-1) */
            seq_t next;
            if (is_prime(i)) {
                next = next_prime_after(prev);
            } else {
                next = next_composite_after(prev);
            }
            cache[i - 1] = reverse_base3(next);
        }
        max_computed = n;
    }
    return cache[n - 1];
}
