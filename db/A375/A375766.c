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
 * Helpers:   is_fib_uint64
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
#include "../utils/is_fib_uint64.h"

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
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {          /* allocation failure – clean up and return 0 */
                    free(cache);
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
