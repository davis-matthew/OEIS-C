/*
 * A362980  Numbers whose multiset of prime factors (with multiplicity) has
 *          different median from maximum.
 * https://oeis.org/A362980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer whose multiset of prime factors *
 *            (with multiplicity) has a median different from its maximum
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   build_spf, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"      /* required by build_spf for example */
#include "../utils/factor_u64.h"   /* ensure factor_u64 prototype */
#include "../utils/build_spf.h"    /* build_spf function */

/* static cache for the sequence values */
static seq_t *cache = NULL;          /* array of cached terms */
static size_t cache_len = 0;         /* number of cached terms */
static size_t cache_cap = 0;         /* capacity of 'cache' */

/* static state for generating terms */
static idx_t cur_val = 5;           /* last checked integer (start from 5) */
static idx_t spf_limit = 300000;    /* current limit of SPF array */
static idx_t *spf = NULL;           /* smallest prime factor array */

/* forward declaration of helper */
static bool qualifies(idx_t m);

/* append value to cache, growing if necessary */
static void append_cache(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* normalize to ensure SPF exists for all values up to 'need' */
static void ensure_spf(idx_t need)
{
    if (need <= spf_limit) return;
    /* extend spf array: rebuild from scratch up to new limit */
    idx_t new_limit = spf_limit;
    while (new_limit < need) new_limit *= 2;
    idx_t *new_spf = NULL;
    build_spf(new_limit, &new_spf);
    free(spf);
    spf = new_spf;
    spf_limit = new_limit;
}

/* determine if integer m satisfies the median != maximum condition */
static bool qualifies(idx_t m)
{
    /* factor m into ascending prime list */
    /* maximum possible multiplicity for numbers <= 10^6 is < 20 */
    int factors[32];
    int cnt = 0;
    idx_t tmp = m;
    while (tmp > 1) {
        idx_t p = spf[tmp];
        factors[cnt++] = (int)p;
        tmp /= p;
    }
    if (cnt < 2) return false;   /* primes and prime powers fail */

    int maxp = factors[cnt - 1];
    if (cnt & 1) { /* odd length */
        int median = factors[cnt / 2];
        return median != maxp;
    } else { /* even length */
        int mid1 = factors[cnt / 2 - 1];
        int mid2 = factors[cnt / 2];
        if (mid1 == mid2 && mid1 == maxp) return false; /* median == max */
        return true;
    }
}

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (!cache) {
        /* cache empty, build initial SPF */
        build_spf(spf_limit, &spf);
    }

    /* extend cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        ++cur_val;
        ensure_spf(cur_val);
        if (qualifies(cur_val))
            append_cache((seq_t)cur_val);
    }
    return cache[n - 1];
}
