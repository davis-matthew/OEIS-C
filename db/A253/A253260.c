/*
 * A253260  Brazilian squares.
 * https://oeis.org/A253260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth square that is a Brazilian number (i.e. can be
 *            written * as a repdigit in some base 2 ≤ b ≤ a−2 with digit d, 1
 *            ≤ d < b). * The term for index n ≥ 1 is obtained by generating
 *            squares * and checking each for the Brazilian property. See
 *            function * is_brazilian_square for the test logic
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 405, u64 n <= 405,
 *            u128 n <= 405, big any n
 * Validated: standard, pass, 68 terms through n = 405 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether a square s > 4 is Brazilian.
 * Even squares > 4 are automatically Brazilian.
 * For odd squares, we test:
 *   1) k = 2 case by checking divisors (d*(b+1) = s).
 *   2) k ≥ 3 case by iterating valid bases b up to sqrt(s)
 *      and constructing repdigits of length ≥ 2.
 *
 * The function uses only integer operations:
 *   + - * / %   and comparisons.
 * ------------------------------------------------------------------ */
static bool is_brazilian_square(seq_t s)
{
    /* Exclude 0,1,4, and all odd squares that are not Brazilian. */
    if (s <= 4) return false;

    /* Even squares > 4 are always Brazilian. */
    if ((s & 1) == 0) return true;

    /* ------------------------------------------------------------------ */
    /* 1) Check the k = 2 case: s = d * (b + 1).                            */
    /* ------------------------------------------------------------------ */
    for (idx_t d = 1; (seq_t)d <= s / (seq_t)d; ++d) {
        if (s % d != 0) continue;

        /* Complementary divisor. */
        seq_t d2 = s / (seq_t)d;

        /* First factor pair (d, d2).   */
        seq_t b  = d2 - 1;            /* b = s/d - 1 */
        if (b >= 2 && b <= s - 2 && (seq_t)d < b) return true;

        /* Second factor pair (d2, d). */
        seq_t b2 = (seq_t)d - 1;      /* b = d - 1 */
        if (b2 >= 2 && b2 <= s - 2 && d2 < b2) return true;
    }

    /* ------------------------------------------------------------------ */
    /* 2) Check k ≥ 3: iterate bases b up to sqrt(s).  For each base
     * construct repdigits of increasing length until they exceed s.
     * For each length, if s is divisible by the repdigit, check
     * that the quotient (digit d) is < b.
     * ------------------------------------------------------------------ */
    for (idx_t b = 2; (seq_t)b * b <= s; ++b) {
        seq_t rep = 0;
        for (int k = 1; ; ++k) {
            rep = rep * b + 1;        /* build b^k-1/(b-1) */
            if (rep > s) break;
            if (k < 2) continue;     /* repdigit requires at least 2 digits */

            if (s % rep == 0) {
                seq_t d = s / rep;
                if (d < (seq_t)b) return true;
            }
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Cache to store previously generated terms.                          */
/* ------------------------------------------------------------------ */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static idx_t  terms    = 0;          /* number of terms stored (≥ 0) */
static idx_t  next_m   = 2;          /* next integer to try (m^2) */

/* Ensure the cache has capacity at least 'needed'. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) exit(1);  /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Public function that returns the nth Brazilian square (n ≥ 1).     */
/* The function memoizes results, so calls in any order are fast.    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for 0, return 0 */

    /* Generate terms until we reach the requested index. */
    while (terms < n) {
        seq_t m = (seq_t)next_m;
        seq_t square = m * m;
        if (is_brazilian_square(square)) {
            ensure_cache_capacity(terms + 1);
            cache[terms++] = square;
        }
        ++next_m;
    }
    return cache[n - 1];
}
