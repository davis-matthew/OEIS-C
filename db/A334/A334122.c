/*
 * A334122  a(n) is the sum of all primes <= n, mod n.
 * https://oeis.org/A334122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = (sum of all primes ≤ n) mod n
 * Range:     u8 n <= 42, u16 n <= 1527, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper function: trial division primality test using known primes */
static bool is_prime_uint64(uint64_t m,
                            const uint64_t *primes, size_t prime_len)
{
    if (m < 2)
        return false;
    for (size_t i = 0; i < prime_len; ++i)
    {
        uint64_t p = primes[i];
        if (p * p > m)
            break;
        if (m % p == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Static state: list of primes and running cumulative sums */
    static uint64_t *primes = NULL;
    static seq_t  *cum    = NULL;
    static size_t  prime_cap = 0;
    static size_t  prime_len = 0;
    static idx_t   max_n = 0;

    /* Grow capacity helper */
    if (prime_cap == 0)
    {
        prime_cap = 16;
        primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        cum    = (seq_t  *)malloc(prime_cap * sizeof(seq_t));
    }

    /* Extend state up to n if necessary */
    if (n > max_n)
    {
        for (idx_t m = max_n + 1; m <= n; ++m)
        {
            if (is_prime_uint64((uint64_t)m, primes, prime_len))
            {
                /* Ensure capacity for new prime */
                if (prime_len == prime_cap)
                {
                    prime_cap *= 2;
                    primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
                    cum    = (seq_t  *)realloc(cum,    prime_cap * sizeof(seq_t));
                }
                primes[prime_len] = (uint64_t)m;
                /* Compute cumulative sum for this prime */
                if (prime_len == 0)
                    cum[prime_len] = (seq_t)m;
                else
                    cum[prime_len] = cum[prime_len - 1] + (seq_t)m;
                ++prime_len;
            }
        }
        max_n = n;
    }

    /* Sum of primes ≤ n */
    seq_t sum;
    if (prime_len == 0 || primes[prime_len - 1] > n)
    {
        /* Find last prime ≤ n */
        size_t idx = 0;
        /* binary search */
        size_t lo = 0, hi = prime_len;
        while (lo < hi)
        {
            size_t mid = (lo + hi) / 2;
            if (primes[mid] <= n)
                lo = mid + 1;
            else
                hi = mid;
        }
        if (lo == 0)
            sum = (seq_t)0;
        else
            sum = cum[lo - 1];
    }
    else
    {
        sum = cum[prime_len - 1];
    }

    /* Modulo n */
    seq_t result = sum % (seq_t)n;
    return result;
}
