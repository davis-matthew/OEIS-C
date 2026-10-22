/*
 * A345583  Numbers that are the sum of eight fourth powers in eight or more
 *          ways.
 * https://oeis.org/A345583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3986, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence lists numbers that can be written as a sum of eight
   fourth powers in at least eight distinct ways (order of summands
   irrelevant, repetitions allowed).  The function builds an increasing
   list of such numbers by dynamic programming over the set of fourth
   powers.  It grows the range until it contains at least the requested
   amount of terms. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t   _search_limit = 2000000;   /* current upper bound for DP      */
static seq_t * _terms        = NULL;      /* cached list of found terms      */
static size_t  _termcnt      = 0;         /* number of cached terms          */
static bool    _initialized  = false;     /* cache ready flag                */

/* Rebuild the term cache so that at least `needed` terms are available. */
static void recompute_terms(idx_t needed)
{
    /* Expand the search bound until we gather enough terms. */
    for (;;)
    {
        size_t stride = (size_t)_search_limit + 1;    /* number of sums (0..limit) */
        size_t total  = stride * 9;                  /* nine layers 0..8          */
        uint8_t *dp = (uint8_t *)calloc(total, 1);
        if (!dp) return;           /* allocation failure – leave cache unchanged */

        /* dp[layer][sum] : number of ways to reach `sum` with `layer`
           summands, capped at 8.  dp[0][0] = 1. */
        dp[0] = 1;

        /* Pre‑compute all fourth powers <= _search_limit.               */
        size_t pcount = 0;
        idx_t k = 1;
        while (1) {
            unsigned long long temp = (unsigned long long)k * k * k * k;
            if (temp > (unsigned long long)_search_limit) break;
            ++pcount;
            ++k;
        }
        /* Store weights so we don't recompute them. */
        unsigned long long *weights = (unsigned long long *)malloc(pcount * sizeof(unsigned long long));
        if (!weights) { free(dp); return; }
        k = 1;
        for (size_t i = 0; i < pcount; ++i) {
            unsigned long long temp = (unsigned long long)k * k * k * k;
            weights[i] = temp;
            ++k;
        }

        /* DP update: for each weight, add its contribution to every
           higher layer (for p = 1..8). */
        for (size_t widx = 0; widx < pcount; ++widx) {
            size_t weight = (size_t)weights[widx];
            for (size_t p = 1; p <= 8; ++p) {
                size_t base      = p * stride;
                size_t prev_base = (p - 1) * stride;
                /* Update sums from weight up to limit.           */
                for (size_t s = weight; s <= (size_t)_search_limit; ++s) {
                    uint8_t prev = dp[prev_base + s - weight];
                    if (!prev) continue;
                    uint8_t cur  = dp[base + s];
                    uint8_t sum  = cur + prev;
                    dp[base + s] = (sum > 8 ? 8 : sum);
                }
            }
        }
        free(weights);

        /* Count how many sums have at least eight representations. */
        size_t cnt = 0;
        for (size_t s = 0; s <= (size_t)_search_limit; ++s)
            if (dp[8 * stride + s] >= 8) ++cnt;

        /* Allocate/reallocate cache for those terms. */
        seq_t *terms = (seq_t *)realloc(_terms, cnt * sizeof(seq_t));
        if (!terms) { free(dp); return; }      /* failure – keep old cache */
        _terms  = terms;
        _termcnt = cnt;

        /* Fill the cache in increasing order of the sum. */
        size_t idx = 0;
        for (size_t s = 0; s <= (size_t)_search_limit; ++s)
            if (dp[8 * stride + s] >= 8)
                _terms[idx++] = (seq_t)s;

        /* Clean up. */
        free(dp);
        _initialized = true;

        if (_termcnt >= (size_t)needed) return;   /* enough terms gathered */

        /* Not enough terms: enlarge the bound and try again. */
        size_t new_limit = _search_limit * 2;
        if (new_limit < _search_limit) new_limit = SIZE_MAX / 2; /* overflow guard */
        _search_limit = new_limit;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (!_initialized || _termcnt < (size_t)n)
        recompute_terms(n);
    if (n > _termcnt) return 0;  /* safety – should never happen */
    return _terms[n - 1];
}
