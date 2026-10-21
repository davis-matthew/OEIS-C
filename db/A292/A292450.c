/*
 * A292450  Numbers where 0 outnumbers any other digit.
 * https://oeis.org/A292450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 2403, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_more_zero_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A292450: numbers where digit 0 outnumbers every other digit.
    The sequence is enumerated in increasing order by scanning all
    non‑negative integers and selecting those that satisfy the property. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_more_zero_digits.h"

/* Ensure the dynamic array has space for at least "need" elements. */
static void
ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (need > *cap) {
        size_t newcap = (*cap) ? *cap * 2 : 16;
        while (newcap < need)
            newcap *= 2;
        *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/*  A292450(n)  –  n ≥ 1  (offset 1)  */
seq_t a(idx_t n)
{
    /* Static state to allow non‑monotonic calls. */
    static idx_t next = 0;             /* next integer to test        */
    static size_t count = 0;           /* how many qualifying numbers found */
    static seq_t *seq = NULL;          /* dynamic array of results     */
    static size_t cap = 0;             /* current capacity of seq array */

    /* Advance until we have at least n terms computed. */
    while (count < n) {
        if (has_more_zero_digits(next)) {
            ensure_capacity(&seq, &cap, count + 1);
            seq[count] = (seq_t)next;
            count++;
        }
        next++;
    }

    return seq[n - 1];
}
