/*
 * A319352  a(n) = Product_{d|n, d<n} prime(1+A056239(d)), where A056239(d)
 *          gives the weight of the partition whose Heinz-number is d.
 * https://oeis.org/A319352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d<n} prime(1 + A056239(d)), where
 *            A056239(d) = sum_{p^e||d} (index(p)) * e (index(p) = 1 for prime
 *            2, 2 for prime 3, …)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 83, u64 n <= 335,
 *            u128 n <= 1259, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/_floor_isqrt_u64.h"

/* Cached list of primes up to the largest n processed. */
static uint64_t *prime_list = NULL;
static size_t    prime_count = 0;
static idx_t    prime_limit  = 0;

/* Build a list of all primes <= limit and store it in prime_list. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;                  /* we already have enough */

    if (prime_list)
        free(prime_list);

    build_primes_upto(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* Binary search to find the index (1‑based) of prime p in prime_list. */
static size_t
prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* After the loop lo is the count of primes <= p; since p is known to be prime
       and present in the list, its 1‑based index is lo. */
    return lo;
}

/* Compute A056239(d): sum of (prime index)*exponent over all prime factors of d. */
static size_t
a056239(uint64_t d)
{
    if (d == 1)
        return 0;                /* weight of the empty partition */

    size_t w = 0;
    uint64_t rem = d;

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= rem; ++i) {
        uint64_t p = prime_list[i];
        if (rem % p == 0) {
            size_t exp = 0;
            do {
                rem /= p;
                ++exp;
            } while (rem % p == 0);
            /* index(p) is 1‑based: i+1 */
            w += (i + 1) * exp;
        }
    }

    if (rem > 1) {            /* remaining prime factor > sqrt(d) */
        size_t idx = prime_index(rem);
        w += idx;              /* exponent is 1 */
    }

    return w;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;         /* a(1) = 1 (empty product) */

    /* Ensure we have all primes up to n to compute A056239 and to access primes by index. */
    ensure_primes(n);

    seq_t prod = (seq_t)1;
    idx_t root = _floor_isqrt_u64(n);

    for (idx_t i = 1; i <= root; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            if (i < n) {
                size_t w = a056239((uint64_t)i);
                /* prime number at index w (0‑based in prime_list) */
                prod *= (seq_t)prime_list[w];
            }

            /* divisor d2 = n / i */
            idx_t d2 = n / i;
            if (d2 != i && d2 < n) {
                size_t w = a056239((uint64_t)d2);
                prod *= (seq_t)prime_list[w];
            }
        }
    }
    return prod;
}
