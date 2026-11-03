/*
 * A375766  The maximum exponent in the prime factorization of the numbers
 *          whose exponents in their prime factorization are all Fibonacci
 *          numbers.
 * https://oeis.org/A375766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9825 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A375766
 *  Sequence:  a(n) = maximum exponent in the prime factorisation of the
 *             n-th positive integer whose prime exponents are all
 *             Fibonacci numbers.
 *  No simple closed formula; the function enumerates the integers in
 *  increasing order, tests each for the Fibonacci-exponent property,
 *  stores the maximum exponent for the qualifying numbers and returns
 *  the value for the requested index n.  The implementation uses
 *  trial division for factorisation; the numbers involved for
 *  n ≤ 10000 are well below 64 bit, so the algorithm is fast.
 *  It is fully valid C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* --------- small helper: check if an integer is a Fibonacci number --------- */
static bool is_fib_uint64(uint64_t e)
{
    /* Fibonacci numbers up to 219330 (which exceeds any exponent we will
       encounter for n ≤ 10000). */
    static const uint64_t fibs[] = {
        0ULL, 1ULL, 1ULL, 2ULL, 3ULL, 5ULL, 8ULL, 13ULL, 21ULL,
        34ULL, 55ULL, 89ULL, 144ULL, 233ULL, 377ULL, 610ULL,
        987ULL, 1597ULL, 2584ULL, 4181ULL, 6765ULL, 10946ULL,
        17711ULL, 28657ULL, 46368ULL, 75025ULL, 121393ULL,
        196418ULL, 317811ULL, 514229ULL, 832040ULL, 1346269ULL,
        2178309ULL, 3524578ULL, 5702887ULL, 9227465ULL,
        14930354ULL, 24157819ULL, 39088173ULL, 63245992ULL,
        102334155ULL, 165580141ULL, 267914296ULL, 433494437ULL,
        701408733ULL, 1134903170ULL, 1836311903ULL, 2971215073ULL,
        4807526976ULL, 7778742049ULL
    };
    for (size_t i = 0; i < sizeof(fibs)/sizeof(fibs[0]); ++i) {
        if (fibs[i] == e) return true;
        if (fibs[i] > e) break;
    }
    return false;
}

/* --------- test if a number x (uint64_t) has all exponents Fibonacci --------- */
static bool good_number(uint64_t x, seq_t *max_exp)
{
    seq_t maxE = 0;

    if (x == 1) {
        *max_exp = 0;
        return true;            /* 1 has no prime factors → max exponent 0 */
    }

    uint64_t tmp = x;
    unsigned e;

    /* factor 2 */
    e = 0;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        ++e;
    }
    if (e > 0) {
        if (!is_fib_uint64(e)) return false;
        if ((seq_t)e > maxE) maxE = e;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            if (!is_fib_uint64(e)) return false;
            if ((seq_t)e > maxE) maxE = e;
        }
    }

    /* remaining prime factor, if any, has exponent 1 (always Fibonacci) */
    if (tmp > 1) {
        if (1 > maxE) maxE = 1;
    }

    *max_exp = maxE;
    return true;
}

/* --------------------------------------------------------------------------- */
/*  Sequence a(idx_t n)                                                    */
/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache   = NULL;   /* array of computed terms (1‑based index) */
    static size_t capacity = 0;     /* allocated size of cache  */
    static size_t count    = 0;     /* how many terms are stored */
    static uint64_t next_candidate = 1;  /* next integer to test */

    if (n == 0) return 0;  /* harmless guard – OEIS offset is 1 */

    /* Grow the cache to hold all terms up to n */
    while (count < (size_t)n) {
        seq_t maxE;
        if (good_number(next_candidate, &maxE)) {
            /* add the result to the cache */
            if (count == capacity) {
                size_t new_cap = capacity ? capacity * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) {          /* allocation failure – clean up and return 0 */
                    mem_free(cache);
                    cache = NULL;
                    capacity = 0;
                    count = 0;
                    return 0;
                }
                cache = tmp;
                capacity = new_cap;
            }
            cache[count] = maxE;
            ++count;
        }
        ++next_candidate;
    }

    return cache[(size_t)(n - 1)];
}
