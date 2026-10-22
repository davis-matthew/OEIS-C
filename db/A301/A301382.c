/*
 * A301382  a(1) = 1. For n > 1, a(n) is the smallest positive integer x not
 *          already in the sequence such that the product of x and its initial
 *          digit is minimal and strictly larger than the same product for any
 *          previous term.
 * https://oeis.org/A301382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 206, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A301382
 *   a(1) = 1.
 *   For n > 1, a(n) is the smallest positive integer x not already in the
 *   sequence such that the product P = x * first_digit(x) is minimal and
 *   strictly larger than the previous product.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* first_digit returns the most significant decimal digit of x */
static seq_t first_digit(seq_t n)
{
    while (n >= 10)
        n /= 10;
    return n;
}

/* dynamic array of already generated terms */
static seq_t *seq_vals      = NULL;   /* array of terms */
static idx_t  seq_len       = 0;      /* current number of stored terms */
static idx_t  seq_cap       = 0;      /* capacity of seq_vals */

/* ensure seq_vals can hold at least 'needed' elements (0‑based) */
static void seq_ensure_capacity(idx_t needed)
{
    if (needed <= seq_cap)
        return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap <= needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* allocation failure */
    seq_vals = tmp;
    seq_cap  = new_cap;
}

/* linear search: true iff value x is already in the sequence */
static bool used_is_set(seq_t x)
{
    for (idx_t i = 0; i < seq_len; ++i)
        if (seq_vals[i] == x)
            return true;
    return false;
}

/* generate terms up to and including index 'n' (1‑based) */
static void generate_up_to(idx_t n)
{
    if (n == 0)
        return;

    if (seq_len == 0) {          /* initialise with a(1) = 1 */
        seq_ensure_capacity(1);
        seq_vals[0] = (seq_t)1;
        seq_len     = 1;
    }

    for (idx_t idx = seq_len; idx < n; ++idx) {
        seq_t prev_val   = seq_vals[idx - 1];
        seq_t prev_prod  = prev_val * first_digit(prev_val);
        unsigned __int128 p = (unsigned __int128)prev_prod + 1;

        seq_t chosen = 0;
        while (chosen == 0) {
            /* try all possible first digits for this product */
            seq_t best_x = 0;
            for (int f = 1; f <= 9; ++f) {
                if (p % f != 0)
                    continue;
                seq_t x = (seq_t)(p / f);
                if (first_digit(x) != (seq_t)f)
                    continue;
                if (used_is_set(x))
                    continue;
                if (best_x == 0 || x < best_x)
                    best_x = x;
            }
            if (best_x != 0) {
                chosen = best_x;
                break;
            }
            ++p;   /* next product */
        }

        seq_ensure_capacity(idx + 1);
        seq_vals[idx] = chosen;
        seq_len++;
    }
}

/* public interface required by the harness */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* undefined before the offset */
    generate_up_to(n);
    return seq_vals[n - 1];
}
