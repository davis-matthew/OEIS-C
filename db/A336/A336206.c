/*
 * A336206  Lexicographically earliest sequence of nonnegative terms such that
 *          whenever a(k_1) = ... = a(k_m) with k_1 < ... < k_m, the sum k_1 +
 *          ... + k_m can be computed without carries in base 10.
 * https://oeis.org/A336206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 462, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A336206: 
 * The lexicographically earliest sequence of non‑negative integers such that
 * whenever a(k1) = … = a(km) (with k1 < … < km), the sum k1+…+km can be
 * computed without carries in base 10. */
 
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_DIGITS 20   /* maximum decimal digits of a uint64_t index */

/* storage for the computed terms and per‑value digit sums */
static seq_t      *a_vals      = NULL;      /* a_vals[i] = a(i+1) */
static unsigned char **digit_sums = NULL;   /* digit_sums[v][d] = sum of d‑th digits of all indices assigned value v  */
static size_t           terms_cap = 0;      /* capacity of a_vals array   */
static size_t           values_cap = 0;     /* number of distinct values stored */
static size_t           computed_len = 0;   /* terms up to computed_len are known */

/* --------------------------------------------------------------------- */
/* Utility: convert an idx_t to its decimal digits (least significant first) */
static void get_digits(idx_t x, unsigned char *buf, size_t *len)
{
    size_t i = 0;
    do {
        buf[i++] = (unsigned char)(x % 10);
        x /= 10;
    } while (x);
    *len = i;
}

/* --------------------------------------------------------------------- */
/* Ensure the terms array has room for at least 'min_cap' entries. */
static void ensure_terms_capacity(size_t min_cap)
{
    if (min_cap <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 128;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    a_vals = tmp;
    terms_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Ensure the value storage has room for up to 'min_cap' distinct values
 * (and allocate zeroed digit sum arrays for any newly created values). */
static void ensure_values_capacity(size_t min_cap)
{
    if (min_cap <= values_cap) return;
    size_t new_cap = values_cap ? values_cap << 1 : 8;
    if (new_cap < min_cap) new_cap = min_cap;
    unsigned char **tmp = (unsigned char **)realloc(digit_sums,
                                                    new_cap * sizeof(unsigned char *));
    for (size_t i = values_cap; i < new_cap; ++i) {
        unsigned char *arr = (unsigned char *)calloc(MAX_DIGITS, sizeof(unsigned char));
        if (!arr) abort();           /* out of memory */
        tmp[i] = arr;
    }
    digit_sums = tmp;
    values_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for non‑positive indices */

    /* Grow to cover all requested indices. */
    if ((size_t)n > computed_len) {
        ensure_terms_capacity((size_t)n);

        for (idx_t cur = computed_len + 1; cur <= n; ++cur) {
            unsigned char digits[MAX_DIGITS];
            size_t dl;
            get_digits((idx_t)cur, digits, &dl);

            /* Find the smallest value v that can accept this index. */
            for (size_t v = 0; ; ++v) {
                if (v >= values_cap) ensure_values_capacity(v + 1);

                unsigned char *sums = digit_sums[v];
                bool ok = true;
                for (size_t d = 0; d < dl; ++d) {
                    if (sums[d] + digits[d] > 9) { ok = false; break; }
                }
                if (ok) {
                    /* assign value */
                    a_vals[(size_t)(cur - 1)] = (seq_t)v;

                    /* update running digit sums for this value */
                    for (size_t d = 0; d < dl; ++d) {
                        sums[d] += digits[d];
                    }
                    break;   /* move on to next index */
                }
            }
        }
        computed_len = (size_t)n;
    }
    return a_vals[n - 1];
}
