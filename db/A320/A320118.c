/*
 * A320118  a(1) = a(2) = 1; for n > 2, a(n) = A181819(n) * a(A181819(n)).
 * https://oeis.org/A320118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 2519, u32 n <= 9399, u64 n <= 9399,
 *            u128 n <= 9399, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A320118
   a(1) = a(2) = 1;
   for n > 2, a(n) = A181819(n) * a(A181819(n))
   with A181819(n) = ∏_{p^e∥n} prime[e],
   where prime[1] = 2, prime[2] = 3, prime[3] = 5, ... (the sequence of primes). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* First 20 primes are enough for exponents up to 20 (2^20 > 1e6).  */
/* The exponent of a prime in n (e ≥ 1) yields prime[e] (1‑based). */
static const uint32_t prime_list[20] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};

/* Compute A181819(n) as a seq_t value. */
static seq_t A181819(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t m = n;
    seq_t result = (seq_t)1;

    /* factor 2 separately */
    idx_t e = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        ++e;
    }
    if (e > 0)
        result *= (seq_t)prime_list[e - 1];

    /* odd prime factors */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            result *= (seq_t)prime_list[exp - 1];
        }
    }
    if (m > 1) /* remaining prime with exponent 1 */
        result *= (seq_t)prime_list[0];

    return result;
}

/* ---- Cache for a(n) values ---- */
static seq_t *a_cache = NULL;
static idx_t a_cache_cap = 0;

/* Ensure the cache has capacity for indices up to n. */
static void ensure_cache(idx_t n)
{
    if (a_cache_cap > n)
        return;

    idx_t new_cap = n + 1;
    seq_t *new_ptr;
    if (a_cache) {
        new_ptr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!new_ptr) exit(1);
        memset(new_ptr + a_cache_cap, 0,
               (new_cap - a_cache_cap) * sizeof(seq_t));
    } else {
        new_ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_ptr) exit(1);
        memset(new_ptr, 0, new_cap * sizeof(seq_t));
    }
    a_cache = new_ptr;
    a_cache_cap = new_cap;

    /* initialise base cases if not already done */
    if (a_cache_cap > 2) {
        a_cache[1] = (seq_t)1;
        a_cache[2] = (seq_t)1;
    }
}

/* Recursive computation with memoisation. */
static seq_t a_recursive(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;

    ensure_cache(n);
    if (a_cache[n] != 0)
        return a_cache[n];

    seq_t factor = A181819(n);
    idx_t next = (idx_t)factor;          /* factor <= n for n <= 10000 */
    seq_t val = factor * a_recursive(next);

    a_cache[n] = val;
    return val;
}

/* Public function requested. */
seq_t a(idx_t n)
{
    return a_recursive(n);
}
