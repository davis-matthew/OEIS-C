/*
 * A272326  Primes of the form k^4 + 853*k^3 + 2636*k^2 + 3536*k + 1753 in
 *          order of increasing nonnegative k.
 * https://oeis.org/A272326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime of the form k^4 + 853*k^3 + 2636*k^2 +
 *            3536*k + 1753
 * Range:     u8 none, u16 n <= 4, u32 n <= 73, u64 n <= 2457, u128 n <= 2457,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"

/* Cached primes seen so far. */
static seq_t *prime_cache = NULL;
/* Number of primes stored in prime_cache. */
static size_t prime_cnt = 0;
/* Current capacity of prime_cache. */
static size_t prime_cap = 0;
/* Next value of k to test. */
static idx_t next_k = 0;

/* Ensure prime_cache can hold at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (prime_cap >= need)
        return;
    size_t new_cap = prime_cap ? prime_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    prime_cache = tmp;
    prime_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)                  /* offset is 1, so a(0) is undefined */
        return 0;

    /* Return cached prime if already computed. */
    if (prime_cnt >= n)
        return prime_cache[n - 1];

    /* Generate primes until the nth one is found. */
    while (prime_cnt < n) {
        idx_t   k   = next_k++;          /* current k value (non‑negative) */
        seq_t   ks  = (seq_t)k;           /* k as seq_t */
        seq_t   k2  = ks * ks;            /* k^2 */
        seq_t   k3  = k2 * ks;            /* k^3 */
        seq_t   k4  = k3 * ks;            /* k^4 */

        /* Evaluate the polynomial. */
        seq_t val = k4
                    + (seq_t)853 * k3
                    + (seq_t)2636 * k2
                    + (seq_t)3536 * ks
                    + (seq_t)1753;

        if (isPrimeGeneric(val)) {
            ensure_cache_cap(prime_cnt + 1);
            prime_cache[prime_cnt++] = val;
        }
    }

    return prime_cache[n - 1];
}
