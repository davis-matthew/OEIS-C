/*
 * A295923  Number of twice-factorizations of n where the first factorization
 *          is constant, i.e., type (P,R,P).
 * https://oeis.org/A295923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{e≥1, d^e = n} f(d)^e, where f(d) is the number of
 *            unordered * factorizations of d into integers > 1 (with f(1)=1).
 *            For n = 1, a(1)=1. The * function evaluates the sum by iterating
 *            over all perfect root exponents e, * finding the unique integer
 *            root d, computing f(d) via a recursive coun...
 * Range:     u8 n <= 32, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   _factor_count_rec, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"
#include "../utils/_factor_count_rec.h"

/* --------------------------------------------------------------------- */
/* counting unordered factorizations of n (f(n))                          */

static seq_t *f_cache      = NULL;  /* values of f(n) for n < CACHE_LIMIT          */
static uint8_t *f_computed = NULL;  /* 0 = not computed yet, 1 = computed          */
#define CACHE_LIMIT 10001          /* enough for all n in the test range          */

static seq_t _factor_count(idx_t n)
{
    if (n < CACHE_LIMIT)
    {
        if (f_cache == NULL)
        {
            f_cache      = (seq_t*)malloc(CACHE_LIMIT * sizeof(seq_t));
            f_computed   = (uint8_t*)malloc(CACHE_LIMIT * sizeof(uint8_t));
            for (size_t i = 0; i < CACHE_LIMIT; ++i)
            {
                f_cache[i] = 0;
                f_computed[i] = 0;
            }
        }
        if (f_computed[n])
            return f_cache[n];
        seq_t val = _factor_count_rec(n, 2);
        f_cache[n] = val;
        f_computed[n] = 1;
        return val;
    }
    /* for n ≥ CACHE_LIMIT we compute on demand (unlikely for the test range) */
    return _factor_count_rec(n, 2);
}

/* --------------------------------------------------------------------- */
/* helper: integer e‑th root of n (return 0 if n is not an exact e‑th power) */
/* --------------------------------------------------------------------- */
static idx_t _perfect_root(idx_t n, int e)
{
    if (e == 1)               /* root is n itself */
        return n;

    idx_t low = 2;
    idx_t high = n;           /* n is small (≤1e4), binary search is cheap */
    while (low <= high)
    {
        idx_t mid = low + (high - low) / 2;
        seq_t p = pow_uint((uint64_t)mid, (idx_t)e);   /* value of mid^e */
        if (p == (seq_t)n)
            return mid;
        if (p < (seq_t)n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return 0;                 /* not a perfect e‑th power */
}

/* --------------------------------------------------------------------- */
/* main sequence function                                                 */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1)
        return 1;           /* by definition */

    seq_t ans = 0;

    /* e = 1 terms: n itself */
    ans += _factor_count(n);

    /* higher exponents */
    for (int e = 2; ; ++e)
    {
        /* smallest candidate 2^e; if it already exceeds n, no further e are possible */
        if (pow_uint(2, (idx_t)e) > (seq_t)n)
            break;

        idx_t root = _perfect_root(n, e);
        if (root > 1)
        {
            seq_t f_root = _factor_count((idx_t)root);
            seq_t pow_val = 1;
            for (int i = 0; i < e; ++i)
                pow_val *= f_root;
            ans += pow_val;
        }
    }

    return ans;
}
