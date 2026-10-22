/*
 * A386801  Numbers that have exactly two exponents in their prime
 *          factorization that are equal to 3.
 * https://oeis.org/A386801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 159, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that have exactly two exponents in their prime factorisation that are equal to 3. */
/* This function returns the n‑th such number (a(1) = 216). */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------  Static auxiliary data  ---------- */
static uint64_t      last_checked = 0;          /* highest integer already examined   */
static uint32_t     *spf          = NULL;      /* array of smallest prime factors    */
static size_t       spf_cap      = 0;           /* capacity of the spf array           */
static seq_t       *cache        = NULL;      /* generated terms                     */
static size_t       cache_cap    = 0;           /* capacity of cache                   */
static size_t       cache_len    = 0;           /* number of terms actually stored     */

/* --------------------------------------------------------------------
   Ensure that the smallest prime factor array is defined up to at
   least `limit`.  The array is rebuilt each time `limit` increases.
   -------------------------------------------------------------------- */
static void
ensure_spf (size_t limit)
{
    if (limit <= spf_cap)
        return;

    free (spf);
    spf = (uint32_t *)malloc ((limit + 1) * sizeof (uint32_t));
    if (!spf)
        exit (EXIT_FAILURE);

    for (size_t i = 0; i <= limit; ++i)
        spf[i] = 0;

    spf[1] = 1;
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                 /* i is prime          */
            spf[i] = (uint32_t)i;
            if (i * i <= limit) {          /* sieve multiples     */
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
            }
        }
    }
    spf_cap = limit;
}

/* --------------------------------------------------------------------
   Grow the result cache so that its capacity is at least `need`.
   -------------------------------------------------------------------- */
static void
ensure_cache (size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 128;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_buf = (seq_t *)realloc (cache, new_cap * sizeof (seq_t));
    if (!new_buf)
        exit (EXIT_FAILURE);
    cache     = new_buf;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------
   Test whether an integer `x` (within the 64‑bit range used here)
   has exactly two distinct prime exponents equal to 3.
   -------------------------------------------------------------------- */
static int
has_two_exp_three (uint64_t x)
{
    int cnt3 = 0;
    uint64_t t = x;
    while (t > 1) {
        uint32_t p = spf[t];
        int exp = 0;
        while (t % p == 0) {
            t /= p;
            ++exp;
        }
        if (exp == 3) {
            ++cnt3;
            if (cnt3 > 2)
                return 0;
        }
    }
    return (cnt3 == 2);
}

/* --------------------------------------------------------------------
   Main sequence function required by the framework.
   -------------------------------------------------------------------- */
seq_t
a (idx_t n)
{
    if (n == 0)
        return 0;            /* the framework may never request n == 0 */

    /* Ensure the cache can hold the nth term. */
    ensure_cache ((size_t)n);

    /* If we already have the desired term, return it immediately. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Main loop: keep extending the search until `n` terms are found. */
    /* We'll double the search limit each iteration to keep the cost
       near O(n / density). */
    size_t limit = (last_checked > 0) ? (last_checked << 1) : 1000;   /* initial limit */
    if (limit < 1000)
        limit = 1000;

    while (true) {
        /* Ensure the spf array covers the current limit. */
        ensure_spf (limit);

        /* Scan new numbers in the range (last_checked, limit] */
        uint64_t start = last_checked + 1;
        for (uint64_t x = start; x <= limit; ++x) {
            if (has_two_exp_three (x)) {
                /* Store the term */
                if (cache_len >= cache_cap)
                    ensure_cache (cache_len + 1);
                cache[cache_len++] = (seq_t)x;
                if (cache_len >= (size_t)n) {
                    /* We have reached or passed the desired index;
                       set last_checked to the number just inspected. */
                    last_checked = x;
                    return cache[n - 1];
                }
            }
        }

        /* All numbers up to limit were inspected but not enough terms.
           Extend the search range. */
        last_checked = limit;
        size_t new_limit = limit * 2 + 100;   /* add a small buffer */
        if (new_limit <= limit)              /* in case of overflow */
            new_limit = (size_t)UINT64_MAX / 2;
        limit = new_limit;
    }
}
