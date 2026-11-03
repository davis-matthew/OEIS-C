/*
 * A391591  Numbers whose smallest prime exponent is less than 3 and whose
 *          largest prime exponent exceeds 3.
 * https://oeis.org/A391591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 4901, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 8889 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391591:
   a(n) is the n‑th integer whose prime factorisation contains
   at least one exponent < 3 and at least one exponent > 3.
   (In other words, the number is neither cubefree nor cubefull,
   and neither 4‑free nor 4‑full.) */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Test whether m has a prime exponent < 3 and a prime exponent > 3. */
static bool has_small_and_large_exponent(uint64_t m)
{
    bool small = false;     /* some exponent <= 2  */
    bool large = false;     /* some exponent >= 4  */
    uint64_t n = m;
    uint64_t e = 0;

    /* factor 2 */
    if ((n & 1ULL) == 0) {
        e = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            ++e;
        }
        if (e < 3) small = true;
        if (e > 3) large = true;
        if (small && large) return true;
    }

    /* factor odd primes by trial division */
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            e = 0;
            while (n % d == 0) {
                n /= d;
                ++e;
            }
            if (e < 3) small = true;
            if (e > 3) large = true;
            if (small && large) return true;
        }
    }

    /* remaining prime factor, if any, has exponent = 1 < 3 */
    if (n > 1) small = true;

    return small && large;
}

/* Maximum number of terms we keep in the cache.  The 10 000th term
   easily fits below this bound, so the array is more than large enough
   for the required performance range. */
#define CACHE_SIZE 20000

/* OEIS function a(n) */
seq_t a(idx_t n)
{
    /* static cache of sequence values */
    static seq_t cache[CACHE_SIZE];
    static size_t cache_len = 0;          /* number of terms currently stored */
    static uint64_t checked_upto = 2;     /* last integer examined */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        ++checked_upto;
        if (has_small_and_large_exponent(checked_upto)) {
            if (cache_len < CACHE_SIZE) {
                cache[cache_len++] = (seq_t)checked_upto;
            } else {
                /* In the unlikely event that CACHE_SIZE is exceeded,
                   abort—this should never happen for the problem limits. */
                abort();
            }
        }
    }

    return cache[n - 1];
}
