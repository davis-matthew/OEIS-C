/*
 * A371030  n written in compositorial base.
 * https://oeis.org/A371030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = concatenation of decimal digits of n written in the
 *            compositorial mixed‑radix base. The radices are the composite
 *            numbers 4, 6, 8, 9, 10, 12, … (the largest prime factor of each
 *            radix > 1)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_composite_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Only the standard headers are required.  All arithmetic is performed on
   the exported type seq_t, which may be an integer of any width or an
   arbitrary‑precision integer class. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_composite_uint64.h"

/* ------------------------------------------------------------------ */

/* Helper: number of decimal digits in an unsigned integer. */
static int digit_count_uint64(uint64_t x)
{
    if (x == 0) return 1;
    int c = 0;
    while (x) {
        x /= 10;
        ++c;
    }
    return c;
}

/* Helper: return 10^e as a seq_t value.  The exponent e is small. */
static seq_t pow10_seq_t(idx_t e)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < e; ++i)
        res = res * (seq_t)10;
    return res;
}

/* ------------------------------------------------------------------ */
/* Global cache of composite radices.  It grows as required.           */
static uint64_t *composite_radices = NULL;   /* array of composite numbers */
static size_t   composite_cap    = 0;         /* current allocation size */
static size_t   composite_len    = 0;         /* number of radices stored */

/* Ensure there are at least `needed` radices cached. */
static void ensure_composite_radices(size_t needed)
{
    if (needed <= composite_len) return;

    /* Grow the table until it contains `needed` composite numbers. */
    uint64_t candidate = composite_len ? composite_radices[composite_len - 1] : 3;
    while (composite_len < needed) {
        /* Find next composite number > candidate. */
        do {
            ++candidate;
        } while (!is_composite_uint64(candidate));

        /* Grow the allocation if necessary. */
        if (composite_len == composite_cap) {
            size_t new_cap = composite_cap ? composite_cap * 2 : 64;
            uint64_t *tmp = (uint64_t *)realloc(composite_radices,
                                               new_cap * sizeof(uint64_t));
            if (!tmp) { /* Allocation failed – abort the program */
                abort();
            }
            composite_radices = tmp;
            composite_cap = new_cap;
        }
        composite_radices[composite_len++] = candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                             */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Work with a copy of n so we can modify it. */
    uint64_t x = (uint64_t)n;

    /* A small stack array for the digits.  For n up to 10^12,
       at most ~15 digits are needed. */
    uint64_t digit_buf[32];
    size_t digit_cnt = 0;

    size_t pos = 0;
    while (x > 0) {
        ensure_composite_radices(pos + 1);
        uint64_t radix = composite_radices[pos];
        uint64_t d = x % radix;
        x /= radix;
        digit_buf[digit_cnt++] = d;
        ++pos;
    }

    /* Concatenate the decimal digits of the mixed‑radix representation. */
    seq_t result = (seq_t)0;
    for (size_t i = digit_cnt; i-- > 0;) {
        uint64_t dvalue = digit_buf[i];
        int dlen = digit_count_uint64(dvalue);
        seq_t mult = pow10_seq_t(dlen);
        result = result * mult + (seq_t)dvalue;
    }

    return result;
}
