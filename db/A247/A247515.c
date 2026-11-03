/*
 * A247515  Numbers k such that 2*floor(sqrt(prime(k))) <
 *          floor(2*sqrt(prime(k))).
 * https://oeis.org/A247515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 62, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9998 (2026-11-03)
 * Helpers:   ipow_i32, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247515: numbers k such that 2*floor(sqrt(prime(k))) < floor(2*sqrt(prime(k))) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* n must be >= 1 for meaningful output; n==0 returns 0 */
    if (n == 0) return (seq_t)0;

    /* Static state for memoisation across calls */
    static seq_t *qualifiers = NULL;   /* list of qualifying indices k   */
    static size_t qual_cap   = 0, qual_len = 0;
    static uint64_t *primes = NULL;    /* list of primes found      */
    static size_t primes_cap = 0, primes_len = 0;
    static idx_t  prime_index = 0;                     /* current count of primes   */
    static idx_t  candidate = 2;                       /* next candidate to test   */

    /* Expand the qualifiers array as needed */
    while (qual_len < n)
    {
        /* Test if current candidate is prime */
        bool isprime = true;
        for (size_t i = 0; i < primes_len; ++i)
        {
            uint64_t q = primes[i];
            if (q * q > candidate) break;
            if (candidate % q == 0) { isprime = false; break; }
        }

        if (isprime)
        {
            /* Store new prime */
            if (primes_len == primes_cap)
            {
                primes_cap = primes_cap ? primes_cap * 2 : 4;
                uint64_t *tmp = (uint64_t*)realloc(primes,
                                                   primes_cap * sizeof(uint64_t));
                if (!tmp) return (seq_t)0;
                primes = tmp;
            }
            primes[primes_len++] = candidate;
            ++prime_index; /* Index of this prime (starting from 1) */

            /* Check the OEIS condition for this prime */
            seq_t p_seq      = (seq_t)candidate;
            seq_t s_sq      = floor_sqrt(p_seq);          /* s = floor(sqrt(p)) */
            seq_t threshold = s_sq * s_sq + s_sq + 1;    /* s^2 + s + 1      */
            if (p_seq >= threshold)
            {
                if (qual_len == qual_cap)
                {
                    qual_cap = qual_cap ? qual_cap * 2 : 4;
                    seq_t *tmp = mem_resize(seq_t, qualifiers, qual_cap);
                    if (!tmp) return (seq_t)0;
                    qualifiers = tmp;
                }
                qualifiers[qual_len++] = (seq_t)prime_index;
            }
        }

        /* Advance to the next candidate number (skip even numbers after 2) */
        candidate = (candidate == 2) ? 3 : candidate + 2;
    }

    return qualifiers[n - 1];
}
