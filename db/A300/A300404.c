/*
 * A300404  Smallest integer k such that the largest term in the Goodstein
 *          sequence starting at k is > n.
 * https://oeis.org/A300404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the minimal integer k ≥ 2 such that the largest term in
 *            the Goodstein sequence starting at k is > n
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   add_limit, mul_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/add_limit.h"
#include "../utils/mul_limit.h"

/* Work constant: maximum n we are prepared to answer (≥ 10000). */
#define MAX_N 10000UL

/* Utility: easiest limit-aware exponentiation (base^exp),
   with an early exit if the result would exceed limit. */
static seq_t pow_limit(seq_t base, seq_t exp, seq_t limit)
{
    seq_t result = 1;
    while (exp > 0) {
        if (result > limit / base)
            return limit + 1;                /* signal overflow */
        result *= base;
        exp--;
    }
    return result;
}

/* Recursively replace occurrences of old_base in the hereditary
   base representation of n by new_base, returning the evaluated
   integer value (≤ limit+1 as overflow sentinel). */
static seq_t replace_base(seq_t n, seq_t old_base, seq_t new_base, seq_t limit)
{
    if (n < old_base)
        return n;

    /* find e such that old_base^e ≤ n < old_base^(e+1) */
    seq_t p = old_base;
    seq_t e = 1;
    while (p <= n) {
        if (p > limit / old_base)
            break;            /* p would overflow, so stop */
        p *= old_base;
        e++;
    }
    e--;                          /* e is the floor exponent */
    p = old_base;
    for (seq_t i = 1; i < e; ++i) {
        if (p > limit / old_base)
            break;
        p *= old_base;
    }

    seq_t coef = n / p;
    seq_t rem   = n % p;

    seq_t new_e = replace_base(e, old_base, new_base, limit);
    seq_t new_pow =
        (new_e > limit) ? limit + 1
                        : pow_limit(new_base, new_e, limit);
    seq_t left = mul_limit(coef, new_pow, limit);

    seq_t right = replace_base(rem, old_base, new_base, limit);

    seq_t res = add_limit(left, right, limit);
    return res;
}

/* Compute the maximum term appearing in the Goodstein sequence
   starting from initial k, stopping once we exceed limit (sentinel
   limit+1 indicates a value > limit was seen). */
static seq_t max_goodstein(seq_t k, seq_t limit)
{
    seq_t cur = k;
    seq_t mx = cur;
    idx_t step = 0;

    while (cur > 0) {
        seq_t base = (seq_t)(step + 2);          /* base for this step */
        seq_t val = replace_base(cur, base, base + 1, limit);
        if (val > limit)
            return limit + 1;                   /* we already exceeded */
        cur = val - 1;
        if (cur > mx) mx = cur;
        if (cur > limit)
            return limit + 1;
        step++;
    }
    return mx;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* static cache of maxima for k=2,3,4,... */
    static seq_t *maxVals = NULL;      /* array of maxima */
    static size_t maxCnt = 0;          /* number of entries */
    static size_t cap = 0;             /* allocated capacity */
    static bool initialized = false;

    if (!initialized) {
        /* Precompute until a maximum > MAX_N is found. */
        size_t alloc = 64;
        maxVals = (seq_t *)malloc(alloc * sizeof(seq_t));
        if (!maxVals) exit(1);
        cap = alloc;
        maxCnt = 0;

        seq_t k = 2;
        while (true) {
            seq_t mx = max_goodstein((seq_t)k, (seq_t)MAX_N);
            if (maxCnt >= cap) {        /* grow array */
                size_t newcap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(maxVals, newcap * sizeof(seq_t));
                if (!tmp) exit(1);
                maxVals = tmp;
                cap = newcap;
            }
            maxVals[maxCnt++] = mx;
            if (mx > (seq_t)MAX_N)
                break;
            k++;
        }
        initialized = true;
    }

    /* binary search for smallest k such that max > n */
    size_t lo = 0, hi = maxCnt - 1;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (maxVals[mid] > (seq_t)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    /* k = index + 2 because index 0 corresponds to k=2 */
    return (seq_t)(lo + 2);
}
