/*
 * A352438  Numbers k whose decimal expansion starts with the sum of the last
 *          two digits of k.
 * https://oeis.org/A352438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Sequence: Numbers k whose decimal expansion starts with the sum of the last two digits of k */

static seq_t *terms = NULL;       /* dynamically allocated array of found terms */
static size_t terms_len = 0;      /* number of terms currently stored   */
static size_t terms_cap = 0;      /* current capacity of the array      */
static seq_t last_checked = 10;   /* next candidate to test            */

/* Return the number of decimal digits of x.  Works for all seq_t sizes. */
static inline int digit_len(seq_t x)
{
    int len = 0;
    do {
        ++len;
        x /= 10;
    } while (x != 0);
    return len;
}

/* Compute 10^exp for a small non‑negative exponent exp. */
static inline seq_t pow10(int exp)
{
    seq_t r = 1;
    while (exp-- > 0)
        r *= 10;
    return r;
}

/* Check whether candidate satisfies the required property. */
static bool good_candidate(seq_t x)
{
    seq_t last   = x % 10;
    seq_t second = (x / 10) % 10;
    seq_t sum    = last + second;        /* 0 … 18, fits in seq_t */

    int len_k    = digit_len(x);
    int len_sum  = digit_len(sum);

    /* len_k is always >= len_sum for valid terms, but guard anyway. */
    if (len_k < len_sum)
        return false;

    seq_t pow = pow10(len_k - len_sum);
    seq_t prefix = x / pow;
    return prefix == sum;
}

/* Grow the terms array to hold at least new_len entries. */
static void ensure_capacity(size_t new_len)
{
    if (new_len <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap : 64;
    while (new_cap < new_len)
        new_cap <<= 1;          /* double until sufficient */

    seq_t *new_ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = new_ptr;
    terms_cap = new_cap;
}

/* Generate terms until we have at least n terms stored. */
static void generate_up_to(idx_t n)
{
    if (terms == NULL) {
        terms_cap = 0;
        terms = NULL;
    }
    while (terms_len < (size_t)n) {
        if (good_candidate(last_checked)) {
            ensure_capacity(terms_len + 1);
            terms[terms_len++] = last_checked;
        }
        ++last_checked;
    }
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n==0; behaviour irrelevant */
    generate_up_to(n);
    return terms[n - 1];
}
