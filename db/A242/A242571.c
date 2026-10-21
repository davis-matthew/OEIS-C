/*
 * A242571  Triangular numbers T such that sum of the factorials of digits of
 *          T is semiprime.
 * https://oeis.org/A242571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth triangular number T(n)=n*(n+1)/2 such that the
 *            sum of the factorials of the decimal digits of T(n) is a
 *            semiprime
 * Range:     u8 n <= 8, u16 n <= 115, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*--- Pre‑computed digit factorials -----------------------------------*/
static const uint32_t fact[10] = {
    1,          /* 0! */
    1,          /* 1! */
    2,          /* 2! */
    6,          /* 3! */
    24,         /* 4! */
    120,        /* 5! */
    720,        /* 6! */
    5040,       /* 7! */
    40320,      /* 8! */
    362880      /* 9! */
};

/* Sum of the factorials of the decimal digits of an arbitrary‑precision
   integer `x`.  The result is small (< 8 × 362 880) and fits into a uint32_t. */
static uint32_t digit_factorial_sum(seq_t x)
{
    if (x == (seq_t)0)
        return fact[0];

    uint32_t sum = 0;
    while (x != (seq_t)0) {
        unsigned d = (unsigned)(x % (seq_t)10);
        sum += fact[d];
        x /= (seq_t)10;
    }
    return sum;
}

/* Simple primality test for 32‑bit unsigned integers. */
static bool is_prime_small(uint32_t n)
{
    if (n < 2)
        return false;
    if (n == 2 || n == 3)
        return true;
    if ((n & 1U) == 0U)            /* even numbers >2 are composite */
        return false;

    for (uint32_t d = 3; (uint64_t)d * d <= n; d += 2)
        if (n % d == 0U)
            return false;
    return true;
}

/* Check whether a 32‑bit integer is a semiprime (product of exactly
   two (not necessarily distinct) primes). */
static bool is_semiprime(uint32_t n)
{
    if (n < 4)          /* 1,2,3 are not semiprime */
        return false;

    for (uint32_t d = 2; (uint64_t)d * d <= n; ++d) {
        if (n % d == 0U) {
            uint32_t q = n / d;
            if (q >= 2 && is_prime_small(q))
                return true;   /* d and q are primes */
            return false;      /* divisor found, but not a prime pair */
        }
    }
    return false;          /* n is prime itself */
}

/* Resize a dynamic array of seq_t to at least 'need' elements. */
static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need)
        return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap * 2);
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *arr = tmp;
    *cap = new_cap;
}

/* Cache of the terms found so far. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t next_check = 1;   /* next m to evaluate */

/* ------------------------------------------------------------------ */
/* Public function implementing the sequence.                       */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* not used; offset is 1 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        idx_t m      = next_check++;
        seq_t T      = (seq_t)m * ((seq_t)m + (seq_t)1) / (seq_t)2;
        uint32_t sum = digit_factorial_sum(T);

        if (is_semiprime(sum)) {
            grow_array(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = T;
        }
    }
    return cache[n - 1];
}
