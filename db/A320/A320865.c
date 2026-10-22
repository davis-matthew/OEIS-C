/*
 * A320865  Powers of 2 with initial digit 9.
 * https://oeis.org/A320865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 5, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is 2^k where k is the n‑th exponent such that the first
   decimal digit of 2^k is 9. */

#include <stdlib.h>
#include <math.h>
#include "../utils/pow2.h"

/* high‑precision constants */
static const long double LOG10_2 = 0.3010299956639811952137388947244930267682L;
static const long double LOG10_9 = 0.9542425094393249L;

/* Test whether 2^k starts with the digit 9 */
static int leading_digit_is_9(idx_t k)
{
    long double frac = k * LOG10_2;
    frac -= floorl(frac);                 /* fractional part in [0,1) */
    return frac >= LOG10_9;
}

/* Cache of discovered exponents */
static idx_t *exp_cache      = NULL;     /* array of exponents k */
static size_t exp_cap        = 0;        /* current capacity */
static size_t exp_count      = 0;        /* number of stored exponents */
static idx_t  last_exponent  = 0;        /* last exponent examined */

/* Ensure cache holds at least n exponents */
static void ensure_exponents(idx_t n)
{
    if (exp_count >= (size_t)n)
        return;

    /* enlarge cache if needed */
    if (exp_cap < (size_t)n) {
        size_t new_cap = exp_cap == 0 ? 64 : exp_cap << 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(exp_cache, new_cap * sizeof(idx_t));
        if (!tmp) abort();          /* allocation failure */
        exp_cache = tmp;
        exp_cap   = new_cap;
    }

    for (; exp_count < (size_t)n; ++exp_count) {
        idx_t k = last_exponent + 1;
        while (!leading_digit_is_9(k))
            ++k;
        exp_cache[exp_count] = k;
        last_exponent = k;
    }
}

seq_t a(idx_t n)
{
    /* a(1) is defined; for n==0 behaviour is undefined */
    if (n < 1)
        return (seq_t)0;

    ensure_exponents(n);
    idx_t exp = exp_cache[n - 1];
    return pow2(exp);
}
