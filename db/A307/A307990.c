/*
 * A307990  Number of partitions of n into 2 distinct nonadjacent prime parts.
 * https://oeis.org/A307990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of unordered distinct prime pairs p<q with * p+q=n
 *            and p and q are not consecutive primes
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a prime sieve and binary search to count
 * admissible prime pairs for each n.  It memoises the primes array
 * across calls, so subsequent queries are answered in O(π(n)) time. */

#include <stdlib.h>   /* malloc, free */
#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */

#include "../utils/build_primes_upto.h"   /* build_primes_upto */

static uint64_t *s_primes = NULL;        /* primes <= current limit */
static size_t   s_prime_cnt = 0;          /* number of primes stored   */
static idx_t    s_prime_limit = 0;       /* maximum n for which primes are generated */

/* binary search: returns position of key if found, otherwise s_prime_cnt */
static size_t binary_search(const uint64_t *arr, size_t len, uint64_t key)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] < key)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n < 2)               /* no partition possible for n < 4 anyway */
        return 0;

    /* Ensure we have all primes up to n */
    if (n > s_prime_limit) {
        if (s_primes)
            free(s_primes);
        build_primes_upto(n, &s_primes, &s_prime_cnt);
        s_prime_limit = n;
    }

    seq_t count = 0;
    uint64_t half = (uint64_t)(n / 2);   /* integer division */

    for (size_t i = 0; i < s_prime_cnt; ++i) {
        uint64_t p = s_primes[i];
        if (p > half)           /* p <= q automatically */
            break;

        uint64_t q = (uint64_t)(n - p);
        if (q <= p)              /* distinct and avoid double counting */
            continue;

        /* Check if q is a prime by binary searching the primes array */
        size_t idx_q = binary_search(s_primes, s_prime_cnt, q);
        if (idx_q == s_prime_cnt || s_primes[idx_q] != q)
            continue;           /* q is not prime */

        /* Skip consecutive (adjacent) primes: p and q must not be consecutive */
        if (idx_q == i + 1)
            continue;

        count++;                 /* admissible pair found */
    }

    return count;
}
