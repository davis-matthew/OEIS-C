/*
 * A243458  Primes p such that (p reversed)-3 is also a prime.
 * https://oeis.org/A243458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth prime p such that reverse(p)−3 is prime
 * Range:     u8 n <= 9, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 970 (2026-11-03)
 * Helpers:   is_prime_u64, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Utility functions provided by the build environment */
#include <stdint.h>
#include "../utils/reverse_digits.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Static cache to store the sequence values */
static seq_t *cache = NULL;   /* array of accepted primes     */
static size_t cache_size = 0; /* number of elements stored    */
static size_t cache_cap   = 0; /* allocated capacity           */

/* The largest integer examined so far */
static seq_t last_checked = (seq_t)2; /* start from the first prime */

/* Ensure the cache has at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Fill the cache until it contains at least 'n' terms */
static void extend_cache(idx_t n)
{
    if (cache_size >= (size_t)n)
        return;

    while (cache_size < (size_t)n)
    {
        /* Skip even numbers greater than 2 */
        if (last_checked > (seq_t)2 && (last_checked & (seq_t)1) == (seq_t)0)
        {
            last_checked++;
            continue;
        }

        /* Test primality of the candidate */
        if (is_prime_big(last_checked))
        {
            seq_t rev_val = reverse_digits(last_checked);
            if (rev_val >= (seq_t)3)
            {
                seq_t rev_minus = rev_val - (seq_t)3;
                if (is_prime_big(rev_minus))
                {
                    grow_cache(cache_size + 1);
                    cache[cache_size++] = last_checked;
                }
            }
        }

        last_checked++;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for indices < 1 */
    extend_cache(n);
    return cache[(size_t)(n - 1)];
}
