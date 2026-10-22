/*
 * A354417  a(n) is the numerator of the sum of the reciprocals of the first n
 *          squarefree numbers.
 * https://oeis.org/A354417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the sum of reciprocals of the first n
 *            squarefree numbers
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 33, u128 n <= 61,
 *            big any n
 * Helpers:   build_spf, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"           /* seq_t gcd(seq_t a, seq_t b) */
#include "../utils/build_spf.h"     /* void build_spf(idx_t limit, idx_t **spf_out) */

/* Cache for numerators a(1)..a(N) */
static seq_t *num_cache = NULL;
static size_t num_cap = 0;
static size_t num_len = 0;

/* Cache for squarefree numbers */
static idx_t *sf_cache = NULL;
static size_t sf_cap = 0;
static size_t sf_len = 0;
static idx_t sf_next = 1;              /* next integer to test */

/* Smallest prime factor sieve */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* Current accumulated fraction S = A/B (reduced) */
static seq_t curA = 0;
static seq_t curB = 0;

/* Ensure cache capacity for numerators */
static void grow_num_cache(size_t need)
{
    if (need <= num_cap)
        return;
    size_t new_cap = num_cap ? num_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(num_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* memory exhausted */
    num_cache = tmp;
    num_cap = new_cap;
}

/* Ensure cache capacity for squarefree numbers */
static void grow_sf_cache(size_t need)
{
    if (need <= sf_cap)
        return;
    size_t new_cap = sf_cap ? sf_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    idx_t *tmp = (idx_t *)realloc(sf_cache, new_cap * sizeof(idx_t));
    if (!tmp)
        abort();
    sf_cache = tmp;
    sf_cap = new_cap;
}

/* Ensure the spf array covers at least given limit */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return;
    free(spf);
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* Test whether x is squarefree using the spf array */
static bool is_squarefree(idx_t x)
{
    if (x <= 1)
        return true;
    while (x > 1) {
        idx_t p = spf[x];
        if (p == 0)      /* prime */
            return true;
        int exp = 0;
        while (x % p == 0) {
            x /= p;
            exp++;
            if (exp > 1)
                return false;   /* repeated prime factor found */
        }
    }
    return true;
}

/* Generate squarefree numbers until we have at least n of them */
static void generate_squarefree(idx_t n)
{
    while (sf_len < (size_t)n) {
        idx_t cur = sf_next++;
        if (cur > spf_limit)
            ensure_spf(cur);
        if (is_squarefree(cur)) {
            grow_sf_cache(sf_len + 1);
            sf_cache[sf_len++] = cur;
        }
    }
}

/* Main function as required by the template */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* defined behaviour for n<1 */

    /* Initialization on first call */
    if (num_len == 0) {
        curA = 1;
        curB = 1;
        grow_num_cache(1);
        num_cache[0] = curA;   /* a(1) = 1 */
        num_len = 1;
    }

    /* Extend cache if needed */
    if ((size_t)n > num_len) {
        /* Make sure we have at least n squarefree numbers */
        generate_squarefree(n);

        /* Allocate space for new numerators */
        grow_num_cache((size_t)n);

        /* Compute missing terms */
        for (size_t k = num_len; k < (size_t)n; ++k) {
            idx_t s = sf_cache[k];        /* k-th squarefree (0‑based) */
            seq_t sseq = (seq_t)s;
            seq_t g = gcd(curB, sseq);
            seq_t s_div_g = sseq / g;
            seq_t b_div_g = curB / g;
            seq_t newA = curA * s_div_g + b_div_g;
            seq_t newB = curB * s_div_g;
            seq_t h = gcd(newA, newB);
            curA = newA / h;
            curB = newB / h;
            num_cache[k] = curA;
        }
        num_len = n;
    }

    return num_cache[n - 1];
}
