/*
 * A347303  a(n) = 3^(n-1) - lcm{1..n}.
 * https://oeis.org/A347303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n-1) - lcm{1,2,…,n} (n ≥ 1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   ensure_capacity, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */
#include <stdlib.h>          /* malloc, realloc */
#include "../utils/gcd.h"
#include "../utils/ensure_capacity.h"

static seq_t *lcm_cache = NULL;    /* lcm_cache[i] = lcm{1..i} (1‑based) */
static idx_t  lcm_max  = 0;        /* largest i with lcm_cache[i] computed */
static size_t lcm_cap  = 0;        /* current allocated size */

static seq_t *pow_cache = NULL;    /* pow_cache[i] = 3^(i-1) (1‑based) */
static idx_t  pow_max   = 0;        /* largest i with pow_cache[i] computed */
static size_t pow_cap   = 0;        /* current allocated size */

/* Expand the LCM cache up to n (inclusive).  The cache starts at index 1. */
static void extend_lcm(idx_t n)
{
    if (n <= lcm_max) return;

    size_t needed = (size_t)n + 1;          /* indices 1..n */
    if (needed > lcm_cap) {
        size_t new_cap = lcm_cap > 0 ? lcm_cap * 2 : 1;
        if (new_cap < needed) new_cap = needed;
        ensure_capacity(new_cap, &lcm_cap, (void **)&lcm_cache, sizeof(seq_t));
    }

    if (lcm_max == 0) {
        lcm_cache[1] = 1;                  /* lcm{1} = 1 */
        lcm_max = 1;
    }

    for (idx_t i = lcm_max + 1; i <= n; ++i) {
        seq_t previous = lcm_cache[i - 1];
        seq_t cur      = (seq_t)i;
        seq_t g        = gcd(previous, cur);
        seq_t div      = previous / g;
        lcm_cache[i]   = div * cur;
    }
    lcm_max = n;
}

/* Expand the power cache up to n (inclusive).  pow_cache[1] = 3^(1-1) = 1. */
static void extend_pow(idx_t n)
{
    if (n <= pow_max) return;

    size_t needed = (size_t)n + 1;          /* indices 1..n */
    if (needed > pow_cap) {
        size_t new_cap = pow_cap > 0 ? pow_cap * 2 : 1;
        if (new_cap < needed) new_cap = needed;
        ensure_capacity(new_cap, &pow_cap, (void **)&pow_cache, sizeof(seq_t));
    }

    if (pow_max == 0) {
        pow_cache[1] = 1;                  /* 3^(1-1) = 1 */
        pow_max = 1;
    }

    for (idx_t i = pow_max + 1; i <= n; ++i) {
        pow_cache[i] = pow_cache[i - 1] * (seq_t)3;
    }
    pow_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1; return 0 as safe value */

    extend_lcm(n);
    extend_pow(n);

    seq_t pow_term  = pow_cache[n];   /* 3^(n-1) */
    seq_t lcm_term  = lcm_cache[n];   /* lcm{1..n} */
    return pow_term - lcm_term;
}
