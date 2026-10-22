/*
 * A364058  Heinz numbers of integer partitions with median > 1. Numbers whose
 *          multiset of prime factors has median > 2.
 * https://oeis.org/A364058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A364058 – Heinz numbers of integer partitions whose
   multiset of prime factors has median > 2.
   The sequence is obtained by scanning positive integers and selecting
   those for which the exponent of 2 in the factorization is no more than
   half the total number of prime factors (with multiplicity). */

/* Note: seq_t and idx_t are types supplied by the build system. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

#define MAX_PRIME_LIMIT 200000   /* enough to factor candidates up to the 10 000th term */

static uint64_t *primes = NULL;   /* array of primes up to MAX_PRIME_LIMIT */
static size_t   prime_cnt = 0;

/* Test whether a single integer x satisfies the median condition.
   Returns true if median(prime factors of x) > 2. */
static bool passes(seq_t x)
{
    /* Work with 64‑bit arithmetic because the tested values are far below
       2^64. */
    uint64_t n = (uint64_t)x;
    if (n < 3)
        return false;            /* 1 and 2 never satisfy */

    uint64_t tmp = n;
    uint64_t c2 = 0;   /* exponent of prime 2 */
    uint64_t k  = 0;   /* total number of prime factors (with multiplicity) */

    /* Count factors of 2 */
    while ((tmp & 1ULL) == 0ULL) {
        tmp >>= 1;
        ++c2;
        ++k;
    }

    /* Factor the remaining odd part */
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        while (tmp % p == 0) {
            tmp /= p;
            ++k;
        }
    }

    if (tmp > 1)  /* last prime factor > sqrt(original) */
        ++k;

    /* The sequence includes x if the exponent of 2 is not greater than
       floor(k/2). */
    return c2 <= k / 2;
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Lazily generate the prime list the first time the function is called. */
    if (!primes) {
        build_primes_upto(MAX_PRIME_LIMIT, &primes, &prime_cnt);
    }

    static seq_t   term[20000];          /* cache of found terms */
    static size_t term_size = 0;         /* number of terms stored */
    static uint64_t next_candidate = 2;  /* next integer to test */

    /* Generate terms until we have at least n of them. */
    while (term_size < (size_t)n) {
        ++next_candidate;                /* start from 3, because 2 never qualifies */
        if (passes((seq_t)next_candidate)) {
            term[term_size++] = (seq_t)next_candidate;
        }
    }

    return term[n - 1];
}
