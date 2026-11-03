/*
 * A276001  Numbers n for which A060502(n) <= 1; numbers with at most one
 *          distinct slope in their factorial representation.
 * https://oeis.org/A276001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A276001:
   Numbers n for which all non‑zero digits of the factorial‑base
   representation have the same slope (slope = position − digit).
   This implementation incrementally builds the sequence by testing
   successive integers.  It works for any seq_t implementation.
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Append val to a dynamic array *arr, reallocating if necessary. */
static void _append(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0 ? 16 : (*cap << 1));
        seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
        if (!tmp) abort();                  /* out of memory */
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    ++(*len);
}

/* Check if a factorial‑base representation of num has at most one
   distinct slope among its non‑zero digits. */
static bool _one_slope(seq_t num)
{
    if (num == 0)          /* the special case 0 is always accepted */
        return true;

    seq_t rem = num;      /* remainder while extracting digits */
    bool seen = false;    /* whether a slope has been seen yet */
    seq_t slope_val = 0;  /* the unique slope value, if any */
    idx_t i = 1;          /* current position (1‑based, least significant first) */

    while (rem > 0) {
        seq_t digit = rem % (seq_t)(i + 1);   /* digit for position i */
        rem = rem / (seq_t)(i + 1);           /* quotient for next iteration */

        if (digit > 0) {                     /* consider only non‑zero digits */
            seq_t slope = (seq_t)i - digit;  /* slope at this position */
            if (!seen) {
                seen = true;
                slope_val = slope;
            } else if (slope != slope_val) {
                return false;                /* more than one distinct slope */
            }
        }
        ++i;
    }
    return true;                            /* zero or one distinct slope */
}

/* External function required by the task. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;   /* array holding the sequence values      */
    static size_t len = 0;      /* number of elements stored              */
    static size_t cap = 0;      /* capacity of the array                  */
    static seq_t cand = 0;      /* next integer to test                   */

    /* Generate terms until we have at least (n+1) values. */
    while (len <= (size_t)n) {
        if (_one_slope(cand))
            _append(&seq, &len, &cap, cand);
        ++cand;
    }

    return seq[n];
}
