/*
 * A369347  Numbers whose decimal expansion is quasiperiodic.
 * https://oeis.org/A369347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose decimal representation
 *            (without leading zeros) can be covered by overlapping
 *            occurrences of one of its proper prefixes. The algorithm tests
 *            numbers incrementally, stopping when the required index is
 *            found. It works for any unsigned seq_t larger than 0 and re...
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether x can be covered by one of its proper prefixes.
   Works for arbitrary seq_t values. */
static bool has_quasiperiodic_cover(seq_t x)
{
    /* Extract decimal digits MSB → LSB into a buffer.
       512 digits is more than enough for all numbers tested in the
       allotted range (n up to about 10 000). */
    uint8_t digits[512];
    int len = 0;
    seq_t tmp = x;
    if (tmp == (seq_t)0)
        return false;               /* 0 never occurs in the sequence */

    while (tmp != (seq_t)0) {
        digits[len++] = (uint8_t)(tmp % (seq_t)10);
        tmp = tmp / (seq_t)10;
    }

    /* Reverse to get most‑significant digit first. */
    for (int i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* Try every proper prefix length. */
    for (int p = 1; p < len; ++p) {
        /* Find all start positions of the prefix in the string. */
        int starts[512];
        int startCount = 0;
        starts[startCount++] = 0;           /* prefix always matches at 0 */

        for (int j = 1; j <= len - p; ++j) {
            bool match = true;
            for (int k = 0; k < p; ++k) {
                if (digits[j + k] != digits[k]) {
                    match = false;
                    break;
                }
            }
            if (match)
                starts[startCount++] = j;
        }

        /* Verify that every index is covered by some occurrence. */
        bool ok = true;
        for (int i = 0; i < len; ++i) {
            bool covered = false;
            for (int s = 0; s < startCount; ++s) {
                int st = starts[s];
                if (st <= i && i < st + p) {
                    covered = true;
                    break;
                }
            }
            if (!covered) {
                ok = false;
                break;
            }
        }
        if (ok)
            return true;    /* a suitable prefix exists */
    }
    return false;           /* no covering prefix */
}

/* Main sequence generator with memoisation. */
seq_t a(idx_t n)
{
    /* Persistent state across calls. */
    static seq_t *cache = NULL;   /* array of found terms          */
    static size_t cacheSize = 0;  /* number of terms stored        */
    static size_t cacheCap  = 0;  /* allocated capacity            */
    static seq_t candidate = 0;   /* next value to examine        */
    static bool initialized = false; /* first–call initialisation */
    static idx_t  count    = 0;     /* terms cached so far (count>=0) */

    /* Initialise the first candidate once. */
    if (!initialized) {
        candidate = (seq_t)11;
        initialized = true;
    }

    /* If we have already computed enough terms, return the cached one. */
    if (cacheSize >= n && n > 0)
        return cache[n - 1];

    /* Generate terms until we reach the requested index. */
    while (count < n) {
        if (has_quasiperiodic_cover(candidate)) {
            /* Grow cache if needed. */
            if (cacheSize == cacheCap) {
                size_t newCap = cacheCap == 0 ? 16 : cacheCap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, newCap);
                if (!tmp)                     /* allocation failed */
                    return (seq_t)0;
                cache = tmp;
                cacheCap = newCap;
            }
            cache[cacheSize++] = candidate;
            ++count;

            /* Decide whether to return now; store return value first. */
            seq_t ret = candidate;
            ++candidate;                /* prepare next search value */
            if (count == n)
                return ret;
            continue;                   /* continue searching */
        }
        ++candidate;   /* next integer to test */
    }

    /* The loop above guarantees we have returned the value. */
    return (seq_t)0;
}
