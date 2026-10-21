/*
 * A248954  Let x0 x1 x2...xq denote the decimal expansion of n. Sequence
 *          lists numbers n such that x1/x0 + x2/x1 +...+ x0/xq is an integer.
 * https://oeis.org/A248954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   compute a(n) by brute‑force testing integers until the nth term
 *            satisfying the digit‑ratio sum property is found
 * Range:     u8 n <= 4, u16 n <= 459, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc, seq_free_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_free_array.h"
#include "../utils/seq_realloc.h"

/* Test whether a single integer satisfies the property
   x1/x0 + x2/x1 + ... + x0/xq ∈ ℤ .  Digits are 1–9 only. */
static bool good(seq_t val)
{
    /* Single‑digit numbers trivially satisfy the condition.
       The loop below also handles them correctly. */
    seq_t tmp = val;
    unsigned char rev[64]; /* holds digits in reverse order (least significant first) */
    size_t rev_len = 0;

    while (tmp > (seq_t)0) {
        unsigned char d = (unsigned char)(tmp % (seq_t)10);
        if (d == 0)          /* digit zero makes division impossible */
            return false;
        rev[rev_len++] = d;
        tmp = tmp / (seq_t)10;
    }

    /* Reorder digits to most‑significant first for easier access */
    unsigned char digits[64];
    for (size_t i = 0; i < rev_len; ++i)
        digits[i] = rev[rev_len - 1 - i];

    const seq_t denom_lcm = (seq_t)2520; /* lcm(1..9) */
    seq_t numer = (seq_t)0;

    for (size_t i = 0; i < rev_len; ++i) {
        unsigned char a = digits[(i + 1) % rev_len]; /* numerator of the fraction */
        unsigned char b = digits[i];                /* denominator of the fraction */
        seq_t term_mult = denom_lcm / (seq_t)b;     /* 2520 / b is an integer */
        numer += (seq_t)a * term_mult;               /* accumulate numerator */
    }

    return (numer % denom_lcm) == (seq_t)0;
}

/* Compute the nth term of A248954. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static bool init = false;
    static seq_t next_val = (seq_t)1;

    if (!init) {            /* first call: initialise next_val */
        next_val = (seq_t)1;
        init = true;
    }

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        seq_t candidate = next_val;

        if (good(candidate)) {
            /* Ensure sufficient capacity. */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp)            /* allocation failure – abort */
                    return (seq_t)0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }

        /* Advance to the next integer to test. */
        next_val = next_val + (seq_t)1;
    }

    return cache[n - 1];
}
