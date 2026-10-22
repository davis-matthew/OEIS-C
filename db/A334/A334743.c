/*
 * A334743  a(1) = 1; a(n) = -Sum_{d|n, d < n} omega(n/d) * a(d), where omega
 *          = A001221.
 * https://oeis.org/A334743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{d|n, d < n} omega(n/d) * a(d) (omega = A001221)
 * Range:     i8 n <= 9740, i16 n <= 9740, i32 n <= 9740, i64 n <= 9740,
 *            i128 n <= 9740, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the function uses the helper count_distinct_primes from the
   utils library to obtain omega for numbers up to 64‑bit.  The sequence
   is memoised; it works for arbitrary call order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

static seq_t *cache = NULL;      /* cache of a(n) values (0‑based index) */
static bool  *cached = NULL;     /* flags indicating whether cache[i] is valid */
static size_t cache_cap = 0;     /* current allocated capacity (number of entries) */

/* Ensure the cache can hold values for indices up to (n-1). */
static void ensure_cache_size(idx_t n)
{
    if (n <= cache_cap) return;          /* already enough capacity */

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < (size_t)n) new_cap <<= 1;

    seq_t  *new_cache  = (seq_t *)realloc(cache,  new_cap * sizeof(seq_t));
    bool   *new_cached = (bool  *)realloc(cached, new_cap * sizeof(bool));

    if (!new_cache || !new_cached) {
        /* Allocation failed – abort program as per typical OEIS build policy. */
        exit(EXIT_FAILURE);
    }

    /* Initialise the new flag entries to false. */
    for (size_t i = cache_cap; i < new_cap; ++i) new_cached[i] = false;

    cache   = new_cache;
    cached  = new_cached;
    cache_cap = new_cap;
}

/* Compute a(n) using memoised recurrence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n <= 0, return 0 for safety */

    if (n == 1) return (seq_t)1;   /* base case */

    ensure_cache_size(n);
    if (cached[n-1]) return cache[n-1];

    seq_t sum = 0;

    /* Enumerate all divisors d of n. */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            /* divisor d1 < n */
            if (d1 < n) {
                seq_t od = (seq_t)count_distinct_primes(d2);   /* ω(n/d1) = ω(d2) */
                seq_t ad = a(d1);
                sum += od * ad;
            }

            /* complementary divisor d2 < n, avoid double counting when d1==d2 */
            if (d2 != d1 && d2 < n) {
                seq_t od = (seq_t)count_distinct_primes(d1);   /* ω(n/d2) = ω(d1) */
                seq_t ad = a(d2);
                sum += od * ad;
            }
        }
    }

    seq_t result = -sum;
    cache[n-1] = result;
    cached[n-1] = true;
    return result;
}
