/*
 * A361634  Integers whose number of square divisors is coprime to the number
 *          of their nonsquare divisors.
 * https://oeis.org/A361634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   The sequence A361634 has no known closed form.  
   This implementation generates the terms incrementally: for each integer
   it checks whether the number of square divisors is coprime to the
   number of nonsquare divisors, caches any term that satisfies the
   condition, and returns the n‑th term from the cache.  The function
   handles arbitrary call order by persistent static storage.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---- small utilities ---- */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ---- test whether a number satisfies the condition ---- */
static bool satisfies(uint64_t m)
{
    if (m == 1)
        return true;                       /* 1 has 1 square divisor and 0 nonsquare divisors */

    uint64_t total_divisors = 1;
    uint64_t square_divisors = 1;
    uint64_t n = m;

    for (uint64_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            total_divisors *= (exp + 1);
            square_divisors *= (exp / 2 + 1);
        }
    }
    if (n > 1) {                       /* remaining prime factor */
        total_divisors *= 2;
        square_divisors *= 1;           /* exponent 1 gives 0 even part */
    }

    uint64_t nonsquare_divisors = total_divisors - square_divisors;
    return gcd_uint64(square_divisors, nonsquare_divisors) == 1;
}

/* ---- persistent cache of found terms ---- */
static seq_t *seq_cache = NULL;      /* array of sequence values */
static size_t  seq_cap   = 0;        /* current capacity of seq_cache   */
static size_t  seq_len   = 0;        /* number of terms stored so far  */
static uint64_t next_candidate = 1;  /* next integer to test           */

/* required utilities (provided by the build environment) */
#include "../utils/grow_seq_array.h"

/* ---- the required function ---- */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for n=0, return 0 as a safe default */
        return (seq_t)0;

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        if (satisfies(next_candidate)) {
            if (seq_len == seq_cap) {
                /* grow the array (`grow_seq_array` handles any required construction) */
                grow_seq_array(&seq_cache, &seq_cap, seq_len + 1);
            }
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return seq_cache[n - 1];
}
