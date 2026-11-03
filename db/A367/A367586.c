/*
 * A367586  Numbers whose prime indices have a multiset multiplicity kernel
 *          (in which each prime exponent becomes the least prime factor with
 *          that exponent) that is all ones {1,1,...}. Positions of powers of
 *          2 in A367580.
 * https://oeis.org/A367586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   build_spf, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367586: numbers n such that n is a perfect power
 * with a squarefree base.  The sequence starts with 1, then
 * all even terms of A072774 (powers of squarefree numbers).
 * Implementation scans even integers, tests the condition using
 * a smallest‑prime‑factor array, and memoises the results. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_spf.h"
#include "../utils/mem.h"

/* test whether an even number n satisfies the definition */
static bool is_valid_even(uint64_t n, idx_t *spf_arr)
{
    if ((n & 1) == 1)          /* must be even */
        return false;

    uint64_t temp = n;
    int g = 0;                 /* common exponent */

    while (temp > 1) {
        uint64_t p = spf_arr[(idx_t)temp];
        int e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        if (g == 0)
            g = e;
        else if (e != g)
            return false;      /* exponents not all equal */
    }
    return g > 0;              /* g > 0 automatically because n is even */
}

static seq_t *cache = NULL;       /* sequence terms      */
static size_t cache_len = 0;      /* how many terms stored */
static size_t cache_cap = 0;      /* allocated capacity   */
static idx_t   last_checked = 1;  /* last number tested  */
static idx_t  *spf = NULL;        /* smallest prime factor array */
static idx_t   spf_size = 0;      /* current spf array size */

/* extend the cache until it contains at least target_n terms */
static void extend_cache(idx_t target_n)
{
    while (cache_len < target_n) {
        /* advance to the next even integer */
        do { last_checked++; } while ((last_checked & 1) != 0);

        /* enlarge spf array if needed */
        if (last_checked >= spf_size) {
            idx_t new_limit = last_checked * 2 + 10;
            if (new_limit < 1000) new_limit = 1000;
            idx_t *new_spf = NULL;
            build_spf(new_limit, &new_spf);
            free(spf);
            spf = new_spf;
            spf_size = new_limit;
        }

        /* test for validity */
        if (is_valid_even((uint64_t)last_checked, spf)) {
            /* ensure cache capacity */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n = 0 */

    /* initialise on first call */
    if (!cache) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        cache[0] = (seq_t)1;          /* first term */
        cache_len = 1;
        last_checked = 1;

        /* small spf array for early factors */
        build_spf(10, &spf);
        spf_size = 10;
    }

    if (cache_len < n)
        extend_cache(n);

    return cache[n - 1];
}
