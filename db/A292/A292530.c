/*
 * A292530  Primes prime(k) such that neither prime(k) + prime(k-1) nor
 *          prime(k) + prime(k+1) is divisible by 3.
 * https://oeis.org/A292530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A292530: primes p_k such that neither p_k + p_{k-1} nor p_k + p_{k+1} is divisible by 3.
   The function generates primes on demand, tests the modular condition, and memoises
   all qualifying primes so that any n ≥ 1 can be answered in O(1) after the
   required primes have been generated. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for the primes found so far and for the qualifying primes */
static uint32_t *primes   = NULL;
static size_t   primes_sz = 0, primes_cap = 0;
static uint32_t *good     = NULL;
static size_t   good_sz   = 0, good_cap   = 0;

/* Ensure that primes can hold at least 'need' entries.  Use an explicit cast
   so that the code compiles both as C and as C++. */
static void grow_primes(size_t need)
{
    if (primes_cap < need) {
        size_t newcap = primes_cap ? primes_cap * 2 : 16;
        while (newcap < need) newcap <<= 1;
        primes = (uint32_t *)realloc(primes, newcap * sizeof(uint32_t));
        if (!primes) abort();
        primes_cap = newcap;
    }
}

/* Ensure that good can hold at least 'need' entries.  Same cast as above. */
static void grow_good(size_t need)
{
    if (good_cap < need) {
        size_t newcap = good_cap ? good_cap * 2 : 16;
        while (newcap < need) newcap <<= 1;
        good = (uint32_t *)realloc(good, newcap * sizeof(uint32_t));
        if (!good) abort();
        good_cap = newcap;
    }
}

/* Test whether x is prime using the primes discovered so far. */
static bool is_prime_uint(uint32_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < primes_sz; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Add a new prime to the primes array and, if possible, check the previous
   prime for the A292530 property. */
static void add_prime(uint32_t p)
{
    grow_primes(primes_sz + 1);
    primes[primes_sz++] = p;

    /* When we have at least 3 primes, the middle one becomes eligible for
       evaluation. */
    if (primes_sz >= 3) {
        uint32_t p_prevprev = primes[primes_sz - 3];
        uint32_t p_prev     = primes[primes_sz - 2];
        uint32_t p_curr     = primes[primes_sz - 1];
        if (((p_prev + p_prevprev) % 3) != 0 &&
            ((p_prev + p_curr)     % 3) != 0) {
            grow_good(good_sz + 1);
            good[good_sz++] = p_prev;
        }
    }
}

/* Main sequence function.  Generates primes until the n-th qualifying prime
   is known and returns it as seq_t. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) abort();                  /* sequence is 1‑based */

    /* Ensure we start with the first prime (2). */
    if (primes_sz == 0) add_prime(2);

    /* Keep generating primes until we have at least n good primes. */
    while (good_sz < (size_t)n) {
        /* Find the next prime after the current last prime. */
        uint32_t last = primes[primes_sz - 1];
        uint32_t candidate = last + 1;
        while (!is_prime_uint(candidate))
            ++candidate;
        add_prime(candidate);
    }

    return (seq_t)good[n - 1];
}
