/*
 * A393045  Numbers such that the least prime not dividing their arithmetic
 *          derivative is 5.
 * https://oeis.org/A393045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 9799, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A393045: Numbers whose arithmetic derivative is divisible by
   6 but not by 5 (equivalently, the least prime not dividing the derivative
   is 5).  The function a(idx_t n) returns the n‑th such number, using
   incremental search with memoisation. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: test whether integer n satisfies the condition.
   The arithmetic derivative is evaluated modulo 30, so that we only
   need divisibility by 2 and 3 and non‑divisibility by 5. */
static inline bool ok_uint64(uint64_t n)
{
    if (n < 2)
        return false; /* D(1) = 0, divisible by all primes, not usable */

    uint64_t temp = n;
    uint64_t dmod = 0;          /* D(n) modulo 30 */

    /* Handle factor 2 separately. */
    if ((temp & 1ULL) == 0) {
        uint64_t e = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++e;
        }
        uint64_t term = e * (n >> 1);   /* n / 2 */
        dmod += term % 30;
        if (dmod >= 30)
            dmod -= 30;
    }

    /* Trial division by odd primes. */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            uint64_t term = e * (n / p);
            dmod += term % 30;
            if (dmod >= 30)
                dmod -= 30;
        }
    }

    /* If a prime factor > sqrt remains. */
    if (temp > 1) {
        uint64_t e = 1;
        uint64_t term = e * (n / temp);
        dmod += term % 30;
        if (dmod >= 30)
            dmod -= 30;
    }

    /* dmod now holds D(n) mod 30.  Accept if it is 6,12,18,24. */
    return dmod == 6 || dmod == 12 || dmod == 18 || dmod == 24;
}

/* Dynamic cache of computed sequence values. */
static seq_t *cache  = NULL;
static size_t  cap   = 0;
static size_t  count = 0;        /* number of terms stored   */
static idx_t  next_k = 2;        /* next integer to test     */

/* Grow the cache if necessary. */
static void ensure_cache(size_t need)
{
    if (need <= cap)
        return;
    size_t newcap = cap ? cap * 2 : 16;
    while (newcap < need)
        newcap <<= 1;
    seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = newptr;
    cap   = newcap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for offset 1, but return 0 for safety */

    /* Extend cache to contain at least n terms. */
    while (count < (size_t)n) {
        while (!ok_uint64((uint64_t)next_k))
            ++next_k;   /* skip non‑qualifying numbers */
        ensure_cache(count + 1);
        cache[count++] = (seq_t)next_k;
        ++next_k;
    }
    return cache[n - 1];
}
