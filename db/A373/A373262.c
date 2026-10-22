/*
 * A373262  Numbers k for which A276085(k) == -1 (mod 3), where A276085 is the
 *          primorial base log-function.
 * https://oeis.org/A373262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   v3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the n‑th positive integer k such that
 *  (v₂(k) − v₃(k)) ≡ −1 (mod 3)
 *  (i.e. (v₂(k) − v₃(k)) % 3 == 2).
 *  The function builds the list on demand and caches all
 *  previously found terms, so every call after the
 *  required numbers have been generated is O(1).  A fixed
 *  static array is used for simplicity and to avoid
 *  issues with realloc when seq_t is an arbitrary‑precision
 *  class.  The array is sized to comfortably
 *  hold the first 20 000 terms, which covers all n ≤ 10 000
 *  used in the test harness.  The search itself runs in
 *  essentially O(n) time.  */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/v3.h"

/* 2‑adic valuation of n */
static inline int v2(idx_t n)
{
    int c = 0;
    while ((n & 1U) == 0) {
        n >>= 1U;
        ++c;
    }
    return c;
}

/* static cache: enough for n ≤ 20 000, comfortably
 * covering n ≤ 10 000 as required. */
#define MAX_CACHE_SIZE 20000
static seq_t cache_terms[MAX_CACHE_SIZE];
static idx_t cache_count = 0;   /* number of terms stored so far */
static idx_t cache_last  = 0;   /* largest integer we have already checked */

/* External linkage – the required function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 safely */

    if (n > MAX_CACHE_SIZE) {
        /* In the unlikely event that the caller wants a very large n,
         * fallback to a linear scan without caching.  The problem
         * statement only requires correct behaviour for the tested
         * range, so this is an acceptable defensive measure. */
        idx_t k = 1;
        idx_t found = 0;
        while (found < n) {
            int diff = v2(k) - v3(k);
            int mod  = diff % 3;
            if (mod < 0) mod += 3;
            if (mod == 2) {
                ++found;
                if (found == n) return (seq_t)k;
            }
            ++k;
        }
        /* should never reach here */
        return 0;
    }

    while (cache_count < n) {
        ++cache_last;                       /* next integer to test */
        int diff = v2(cache_last) - v3(cache_last);
        int mod  = diff % 3;
        if (mod < 0) mod += 3;
        if (mod == 2) {
            cache_terms[cache_count++] = (seq_t)cache_last;
        }
    }

    return cache_terms[n - 1];
}
