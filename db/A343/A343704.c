/*
 * A343704  Numbers that are the sum of five positive cubes in three or more
 *          ways.
 * https://oeis.org/A343704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer that can be expressed as the
 *            sum of five positive cubes in at least three distinct unordered
 *            ways
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a DP “coin change” style sieve on cube values.
   dp[d][s] keeps the number of ways to form s with d cubes, capped at 3.
   Whenever a new cube value is added, the DP tables are updated.  The
   list of numbers with dp[5][s] ≥ 3 is kept sorted in a separate dynamic
   array.  The function a(n) grows this list until it contains n terms
   and then returns the (n‑1)st element. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* integer cube root, works for size_t and returns the largest i with i^3 <= n */
static uint64_t int_cube_root(size_t n)
{
    size_t lo = 0, hi = 2097152ULL;            /* 2097152^3 ≈ 9.2e18  */
    while (lo < hi) {
        size_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 m3 = (unsigned __int128)mid * mid * mid;
        if (m3 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* globally persistent state ------------------------------------------------- */
static bool     initialized   = false;
static size_t   dpSize        = 1;          /* number of entries in dp arrays   */
static size_t   maxCubeIndex  = 0;          /* largest cube index processed     */
static uint8_t *dp[6];                     /* dp[d][s] : d = 0..5              */
static seq_t   *terms         = NULL;       /* all numbers with >=3 reps       */
static size_t   termsCap      = 0;
static size_t   termsLen      = 0;

/* push a new term into the sorted list ------------------------------------- */
static void terms_push(seq_t val)
{
    if (termsLen + 1 > termsCap) {
        termsCap = (termsCap == 0) ? 64 : termsCap << 1;
        terms = (seq_t *)realloc(terms, termsCap * sizeof(seq_t));
        if (!terms) exit(EXIT_FAILURE);
    }
    terms[termsLen++] = val;
}

/* expand dp tables up to newSmax and incorporate new cube values ----------- */
static void extend_to(size_t newSmax)
{
    if (newSmax + 1 <= dpSize)
        return;

    size_t oldSize = dpSize;
    dpSize = newSmax + 1;

    /* realign the dp arrays */
    for (int d = 0; d <= 5; ++d) {
        uint8_t *newdp = (uint8_t *)realloc(dp[d], dpSize);
        if (!newdp) exit(EXIT_FAILURE);
        dp[d] = newdp;
        memset(dp[d] + oldSize, 0, dpSize - oldSize);
    }

    /* compute contributions from newly reachable cubes */
    uint64_t newMaxCube = int_cube_root(newSmax);
    for (uint64_t i = maxCubeIndex + 1; i <= newMaxCube; ++i) {
        size_t cube = (size_t)((unsigned __int128)i * i * i);
        for (size_t s = cube; s < dpSize; ++s) {
            for (uint8_t d = 1; d <= 5; ++d) {
                uint8_t prev = dp[d - 1][s - cube];
                if (prev) {
                    uint8_t nv = dp[d][s] + prev;
                    if (nv > 3) nv = 3;
                    dp[d][s] = nv;
                }
            }
        }
    }
    maxCubeIndex = newMaxCube;
}

/* ensure that 'needed' terms are present in the terms array ----------------- */
static void ensure_terms(size_t needed)
{
    while (termsLen < needed) {
        size_t newSmax = (dpSize - 1) + 100000;   /* grow by 100k */
        extend_to(newSmax);

        /* rebuild the terms list from scratch (keeps increasing order) */
        termsLen = 0;
        for (size_t s = 1; s < dpSize; ++s) {     /* ignore s = 0 */
            if (dp[5][s] >= 3)
                terms_push((seq_t)s);
        }
    }
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n = 0 */

    /* one‑time initialization -------------------------------------------- */
    if (!initialized) {
        for (int d = 0; d <= 5; ++d) {
            dp[d] = (uint8_t *)malloc(dpSize * sizeof(uint8_t));
            if (!dp[d]) exit(EXIT_FAILURE);
            memset(dp[d], 0, dpSize);
        }
        dp[0][0] = 1;                  /* 0 can be made with 0 cubes in 1 way */
        initialized = true;
    }

    /* grow the list until we have at least 'n' terms */
    ensure_terms((size_t)n);

    return terms[n - 1];
}
