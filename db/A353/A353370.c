/*
 * A353370  a(n) = 1 if the prime factorization of n contains as many even as
 *          odd prime indices, when counted with multiplicity, otherwise 0.
 * https://oeis.org/A353370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the multiset of prime indices of the prime
 *            factorization of n has equal counts of odd and even indices;
 *            otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, _smallest_prime_factor, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/_smallest_prime_factor.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;
static size_t      prime_cnt  = 0;

/* Ensure prime_list contains all primes up to at least 'limit'. */
static void ensure_primes_limit(idx_t limit)
{
    if (prime_cnt == 0 || prime_list[prime_cnt - 1] < limit)
    {
        uint64_t *new_primes = NULL;
        size_t    new_cnt    = 0;
        build_primes_upto(limit, &new_primes, &new_cnt);
        free(prime_list);
        prime_list = new_primes;
        prime_cnt  = new_cnt;
    }
}

/* Return the 1‑based index of the prime 'p' in prime_list.  The
   list is guaranteed to contain 'p' when called. */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi)
    {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p)
            return mid + 1;   /* 1‑based index */
        else if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* Should never happen if 'p' is prime and in prime_list. */
    return 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t  nn = n;
    /* Initially ensure primes up to sqrt(n)+1 for factorization. */
    ensure_primes_limit(_floor_isqrt_u64(n) + 1);

    size_t even = 0, odd = 0;

    while (nn > 1)
    {
        uint64_t p = _smallest_prime_factor(nn);
        if (p == 0)   /* safety check */
            break;

        /* Make sure p is in the prime list for indexing. */
        ensure_primes_limit(p);

        size_t idx = prime_index(p);
        size_t exp = 0;
        while (nn % p == 0)
        {
            nn /= p;
            ++exp;
        }

        if (idx & 1)
            odd += exp;
        else
            even += exp;
    }

    return (even == odd) ? (seq_t)1 : (seq_t)0;
}
