/*
 * A322987  Number of iterations of A049820(x) = x - A000005(x) needed to
 *          reach a square, when starting from x = n.
 * https://oeis.org/A322987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is a perfect square, otherwise * a(n) = 1 + a(n -
 *            d(n)), where d(n) = number of divisors of n
 * Range:     u8 n <= 63, u16 n <= 8909, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 117 terms through n = 8909 (2026-11-01)
 * Helpers:   build_primes, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
/* Utilities supplied by the build system */
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes.h"
#include "../utils/ipow_i32.h"

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

/* ------------------------------------------------------------------ */
/* Global prime cache                                               */
static idx_t *prime_list      = NULL;  /* dynamic array of primes   */
static size_t prime_count     = 0;      /* number of primes          */
static idx_t  prime_limit     = 0;      /* highest prime required    */

/* ------------------------------------------------------------------ */
/* Ensure that prime_list contains all primes <= limit.               */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;
    if (prime_list) free(prime_list);          /* discard old list */
    build_primes(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Test whether a non‑negative seq_t is a perfect square.             */
static bool is_square(seq_t x)
{
    seq_t s = floor_sqrt(x);
    return s * s == x;
}

/* ------------------------------------------------------------------ */
/* Return the number of positive divisors of an unsigned integer n   */
static seq_t divisor_count(idx_t n)
{
    if (n <= 1) return 1;                     /* d(1)=1, d(0) unused   */
    idx_t root = (idx_t)floor_sqrt((seq_t)n);
    ensure_primes(root);

    seq_t result = 1;
    idx_t temp   = n;

    for (size_t i = 0; i < prime_count; ++i)
    {
        idx_t p = prime_list[i];
        if ((idx_t)p * (idx_t)p > temp) break;
        if (temp % p == 0)
        {
            uint64_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            result *= (seq_t)(exp + 1);
        }
    }
    if (temp > 1)   /* remaining prime factor */
        result *= 2;
    return result;
}

/* ------------------------------------------------------------------ */
/* Public sequence function.                                         */
seq_t a(idx_t n)
{
    if (n == 0 || is_square((seq_t)n)) return 0;
    seq_t steps   = 0;
    idx_t current = n;

    while (!is_square((seq_t)current))
    {
        seq_t d = divisor_count(current);
        current -= (idx_t)d;
        ++steps;
    }
    return steps;
}
