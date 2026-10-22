/*
 * A379832  The second Jordan totient function applied to the exponentially
 *          odd numbers.
 * https://oeis.org/A379832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = J₂(mₙ), where mₙ is the nth number whose prime exponents
 *            are all odd
 * Range:     u8 n <= 12, u16 n <= 183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A379832 – second Jordan totient of the nth exponentially odd number */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"

/* Cache for computed a(n) values: 0‑based indices */
static seq_t *a_vals      = NULL;   /* array of seq_t */
static idx_t  a_count     = 0;      /* number of values stored so far */
static idx_t  a_capacity  = 0;      /* current allocated capacity */

/* Current trial value for the next exponentially odd number */
static uint64_t next_m = 1;

/* Ensure the cache has at least 'needed' slots */
static void grow_vals(size_t needed)
{
    if (needed <= a_capacity)
        return;
    size_t new_cap = a_capacity ? a_capacity * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    a_vals = tmp;
    a_capacity = (idx_t)new_cap;
}

/* Return true if all prime exponents in m are odd (including m = 1) */
static bool is_exp_odd(uint64_t m)
{
    if (m == 1)
        return true;
    uint64_t pr[64], ex[64];
    size_t   cnt;
    factor_u64(m, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i)
        if ((ex[i] & 1u) == 0)   /* even exponent found */
            return false;
    return true;
}

/* Compute J₂(m) = ∏ₚ p^{2(e-1)} (p² – 1)  where m = ∏ₚ pᵉ */
static seq_t J2_of(uint64_t m)
{
    if (m == 1)
        return (seq_t)1;
    uint64_t pr[64], ex[64];
    size_t   cnt;
    factor_u64(m, pr, ex, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p    = (seq_t)pr[i];
        seq_t e    = (seq_t)ex[i];
        seq_t p2   = p * p;          /* p²  */
        seq_t term = p2 - (seq_t)1;  /* p² – 1 */
        seq_t pow_factor = (seq_t)1; /* p^{2(e-1)} */
        for (seq_t j = 1; j < e; ++j)
            pow_factor *= p2;
        result *= term * pow_factor;
    }
    return result;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n = 0, return 0 */

    /* Compute terms until we have a(n) */
    while (a_count < n) {
        /* Find the next exponentially odd number */
        uint64_t m;
        for (;;) {
            m = next_m++;
            if (is_exp_odd(m))
                break;
        }

        /* Compute the value and store it */
        seq_t val = J2_of(m);
        grow_vals(a_count + 1);
        a_vals[a_count++] = val;
    }
    return a_vals[n - 1];
}
