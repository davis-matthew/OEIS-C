/*
 * A257521  Odd Brazilian numbers.
 * https://oeis.org/A257521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A257521: odd Brazilian numbers
 *
 * An odd integer n (n ≥ 7) is Brazilian if there exists a base
 * 2 ≤ b ≤ n-2 such that the representation of n in base b
 * consists of a single repeated digit (length at least 2).
 *
 * The function explores bases up to √n by digit extraction
 * and handles length‑2 repdigits by examining the divisors of n.
 * Results are memoised in a static array so that the function
 * works for calls in any order. */
 
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
 
/* Integer square root for uint64_t using binary search.
 * Returns ⌊√n⌋. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = 1ULL << 32;   /* √(2^64) < 2^32 */
    while (low < high) {
        uint64_t mid = low + ((high - low + 1) >> 1);
        if ((__uint128_t)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}
 
/* Test whether an odd integer n is Brazilian.
 * Handles length‑≥3 repdigits by scanning bases up to √n
 * and length‑2 repdigits by examining divisors of n. */
static bool is_brazilian_odd(uint64_t n)
{
    if (n < 7) return false;          /* 3 and 5 are not Brazilian */
 
    uint64_t limit = isqrt_u64(n);
    /* Check bases 2 … limit (length ≥ 3). */
    for (uint64_t b = 2; b <= limit; ++b) {
        uint64_t tmp = n;
        uint64_t first = tmp % b;
        /* Extract digits; we know there are at least two digits
           because n ≥ b for all bases in this loop. */
        tmp /= b;
        bool same = true;
        while (tmp) {
            if (tmp % b != first) { same = false; break; }
            tmp /= b;
        }
        if (same) return true;
    }
 
    /* Check length‑2 repdigits (b ≥ limit+1). */
    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* First divisor d */
            uint64_t q = d;
            uint64_t b = n / q - 1;
            if (b >= 2 && b <= n - 2 && q < b) return true;
            /* Complementary divisor n/d */
            uint64_t q2 = n / d;
            if (q2 != q) {
                uint64_t b2 = n / q2 - 1;
                if (b2 >= 2 && b2 <= n - 2 && q2 < b2) return true;
            }
        }
    }
    return false;
}
 
/* Return the nth odd Brazilian number (1‑based index). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;     /* number of values stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static uint64_t odd = 1;          /* next odd to test (odd numbers only) */
 
    /* Grow cache until it has at least n elements. */
    while (cache_len < n) {
        /* Allocate or grow the cache. */
        if (cache_cap == 0) {
            cache_cap = 64;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_len == cache_cap) {
            cache_cap <<= 1;          /* double */
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
 
        /* Find the next odd Brazilian number. */
        do {
            odd += 2;                 /* move to next odd */
        } while (!is_brazilian_odd(odd));
 
        cache[cache_len++] = (seq_t)odd;  /* store it */
    }
 
    return cache[n - 1];      /* 1‑based index */
}
