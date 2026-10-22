/*
 * A324854  Lexicographically earliest sequence containing 1 and all positive
 *          integers > 2 whose prime indices already belong to the sequence.
 * https://oeis.org/A324854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lexicographically earliest sequence containing 1 and all
 *            positive integers > 2 whose prime indices already belong to the
 *            sequence
 * Range:     u8 n <= 46, u16 n <= 3050, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Returned by the test harness: a(idx_t n) */
seq_t a(idx_t n)
{
    /* For n = 1 the sequence starts with 1. */
    if (n == 1) return (seq_t)1;

    /* Limit of numbers and primes that will be considered.  This
       size comfortably covers the first 10 000 terms of the
       sequence. */
    const size_t LIMIT = 20000000;   /* 20 million */

    /* Static tables that persist between calls. */
    static bool   *inS            = NULL;  /* membership of numbers in the sequence */
    static uint32_t *prime_index  = NULL;  /* prime_index[p] = ordinal index of prime p  */
    static uint32_t *primes       = NULL;  /* list of primes */
    static size_t   primes_cnt    = 0;
    static size_t   inS_cap       = 0;     /* size of the inS array */

    /* Initialise tables on first call. */
    if (inS == NULL)
    {
        /* Allocate and build prime sieve up to LIMIT. */
        inS_cap = LIMIT + 1;
        inS = (bool*)calloc(inS_cap, sizeof(bool));
        prime_index = (uint32_t*)calloc(inS_cap, sizeof(uint32_t));
        primes = (uint32_t*)malloc((LIMIT/10 + 10) * sizeof(uint32_t));

        /* Eratosthenes sieve. */
        bool *is_prime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
        for (size_t i = 2; i <= LIMIT; ++i) is_prime[i] = true;
        for (size_t i = 2; i * i <= LIMIT; ++i)
            if (is_prime[i])
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_prime[j] = false;

        /* Fill primes list and prime_index mapping. */
        uint32_t rank = 1;
        for (size_t i = 2; i <= LIMIT; ++i)
            if (is_prime[i]) {
                primes[primes_cnt++] = (uint32_t)i;
                prime_index[i] = rank++;
            }
        free(is_prime);

        /* 1 is always in the sequence. */
        inS[1] = true;
    }

    /* Candidate numbers are tested in increasing order, skipping 2. */
    size_t count = 1;               /* a(1) = 1 already counted */
    for (idx_t num = 3; ; ++num)
    {
        if (num > LIMIT)            /* Out of bounds – should never happen for n ≤ 10 000. */
            return 0;

        /* Test whether the current number satisfies the property. */
        bool ok = true;
        idx_t tmp = num;

        /* Trial division by all primes ≤ sqrt(tmp). */
        for (size_t ip = 0; ip < primes_cnt; ++ip)
        {
            uint32_t p = primes[ip];
            if ((idx_t)p * (idx_t)p > tmp) break;
            if (tmp % p == 0)
            {
                /* Prime factor p found – its index must already be in the sequence. */
                if (!inS[ prime_index[p] ]) { ok = false; break; }
                while (tmp % p == 0) tmp /= p;
            }
        }

        /* Possibly remaining prime factor. */
        if (ok && tmp > 1)
            if (!inS[ prime_index[(uint32_t)tmp] ]) ok = false;

        /* Accept the number and check if we have reached the nth term. */
        if (ok)
        {
            inS[(size_t)num] = true;
            if (++count == n)
                return (seq_t)num;
        }
    }
}
