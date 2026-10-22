/*
 * A306104  Numbers that are the difference of two positive squares in at
 *          least four ways.
 * https://oeis.org/A306104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute OEIS A306104: numbers that are the difference of two positive
 * squares in at least four distinct ways.
 *
 * For odd n the number of representations equals floor(tau(n)/2).
 * For n divisible by 4 it equals floor(tau(n/4)/2).  Therefore a term
 * satisfies the condition iff tau(n) ≥ 8 (odd) or tau(n/4) ≥ 8 (n % 4 == 0).
 *
 * The function searches integers sequentially, caches those that meet
 * the criterion, and returns the nth cached value.  It works for any
 * seq_t type, including an arbitrary‑precision integer class when
 * compiled as C++17.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ------------------------------------------------------------------ */
/* Helper: test whether a uint64_t has at least eight positive divisors */
static bool has_at_least_eight_divisors(uint64_t m)
{
    int divs = 1;
    uint64_t tmp = m;
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            divs *= (exp + 1);
            if (divs >= 8)
                return true;
        }
    }
    if (tmp > 1) {           /* remaining prime factor */
        divs *= 2;
        if (divs >= 8)
            return true;
    }
    return divs >= 8;
}

/* ------------------------------------------------------------------ */
/* C++ path: use std::vector for the cache */
#ifdef __cplusplus
static std::vector<seq_t> cache_v;       /* cache of found terms */
static uint64_t next_candidate = 1;      /* next integer to test */

static inline void add_to_cache(uint64_t val)
{
    cache_v.emplace_back((seq_t)val);
}

/* ------------------------------------------------------------------ */
/* C path: use manual dynamic array */
#else
static seq_t *cache = NULL;          /* cache of found terms */
static size_t cache_cnt = 0, cache_cap = 0;
static uint64_t next_candidate = 1;  /* next integer to test */

static void grow_cache(void)
{
    if (cache_cap == 0)
        cache_cap = 64;
    else
        cache_cap <<= 1;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

static inline void add_to_cache(uint64_t val)
{
    if (cache_cnt == cache_cap)
        grow_cache();
    cache[cache_cnt++] = (seq_t)val;
}
#endif

/* ------------------------------------------------------------------ */
/* External function required by the interface. */
seq_t a(idx_t n)
{
    /* Ensure the cache holds at least n elements. */
    while
#ifdef __cplusplus
        (cache_v.size() < n)
#else
        (cache_cnt < n)
#endif
    {
        uint64_t val = next_candidate;

        /* Numbers congruent to 2 (mod 4) cannot be expressed as a
           difference of two squares. */
        if ((val & 3ULL) != 2ULL) {
            uint64_t m = (val & 1ULL) ? val : val / 4ULL;
            if (has_at_least_eight_divisors(m)) {
                add_to_cache(val);
            }
        }
        ++next_candidate;
    }

#ifdef __cplusplus
    return cache_v[(size_t)n - 1];
#else
    return cache[(size_t)n - 1];
#endif
}
