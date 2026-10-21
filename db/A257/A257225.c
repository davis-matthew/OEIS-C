/*
 * A257225  Numbers that have at least one divisor containing the digit 8 in
 *          base 10.
 * https://oeis.org/A257225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer that has at least one divisor *
 *            whose decimal expansion contains the digit 8
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper to test for the presence of digit 8 in a 64‑bit integer. */
static bool has_digit8(uint64_t x)
{
    if (x == 0) return false;
    while (x != 0) {
        if ((x % 10) == 8) return true;
        x /= 10;
    }
    return false;
}

/* Test whether any divisor of k (including k itself) contains digit 8. */
static bool divisor_has_digit8(uint64_t k)
{
    if (has_digit8(k)) return true;
    for (uint64_t d = 1; d * d <= k; ++d) {
        if (k % d == 0) {
            if (has_digit8(d)) return true;
            uint64_t q = k / d;
            if (has_digit8(q)) return true;
        }
    }
    return false;
}

/* Append a value to a dynamic seq_t array, resizing if necessary. */
static void append_value(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len >= *cap) {
        size_t newcap = (*cap == 0) ? 128 : (*cap * 3) / 2;
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (!tmp) abort();    /* memory allocation failed */
        *arr = tmp;
        *cap = newcap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Preserve generated values across calls. */
    static seq_t *vals = NULL;
    static size_t vlen = 0, vcap = 0;
    static uint64_t next_k = 1;   /* Next integer to check */

    if (n < 1) return 0;          /* undefined for 0, but safe */

    /* Generate until we have enough terms. */
    while (vlen < n) {
        if (divisor_has_digit8(next_k)) {
            append_value(&vals, &vlen, &vcap, (seq_t)next_k);
        }
        ++next_k;
    }
    return vals[n - 1];
}
