/*
 * A327246  a(1) = 0; build a(n+1) by concatenating 2 strings: first, the
 *          number of digits of a(n), second, the number of terms with same
 *          number of digits as a(n) up to a(n).
 * https://oeis.org/A327246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 8842 (2026-11-01)
 * Helpers:   num_digits, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A327246:
   a(1) = 0.
   For n ≥ 1, let
        d = number of decimal digits of a(n),
        c = count of terms up to a(n) that have exactly d digits (including a(n)).
   Then a(n+1) is the decimal concatenation of d and c,
   i.e. a(n+1) = d × 10^len(c) + c. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/num_digits.h"
#include "../utils/num_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}
#include <stddef.h>             /* size_t */

#define MAX_DIGITS 64   /* sufficient for 128‑bit values (≤39 digits) */

seq_t a(idx_t n)
{
    if (n < 1)                  /* safety; offset is 1 */
        return 0;

    /* counts of terms seen so far for each digit length */
    size_t counts[MAX_DIGITS] = {0};

    seq_t cur = 0;                /* a(1) */
    unsigned int d = num_digits(cur);   /* digits of 0 is 1 */
    counts[d] = 1;

    if (n == 1)
        return cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* c is the number of terms with digit length d seen so far */
        unsigned int c = (unsigned int)counts[d];

        /* number of decimal digits of c */
        unsigned int clen = num_digits_idx((idx_t)c);

        /* next term = concatenation of d and c */
        seq_t next = (seq_t)d * pow10((idx_t)clen) + (seq_t)c;
        cur = next;

        /* update counters for the new digit length */
        unsigned int dnext = num_digits(cur);
        if (dnext < MAX_DIGITS)
            counts[dnext] += 1;
        else
            /* unlikely for n ≤ 10000; ignore overflow for safety */
            ;

        d = dnext;
    }
    return cur;
}
