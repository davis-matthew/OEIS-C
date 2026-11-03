/*
 * A247584  a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + 3*a(n-5) with
 *          a(0) = a(1) = a(2) = a(3) = a(4) = 1.
 * https://oeis.org/A247584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + 3*a(n-5),
 *            a(0) = a(1) = a(2) = a(3) = a(4) = 1 (OEIS A247584)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 60, u128 n <= 118,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 953 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* Return the n-th term of the sequence.  The function is memoised
   and works for any order of calls. */
seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *tbl = NULL;
    static size_t cap = 0;   /* current allocation size */
    static idx_t  max_n = 0; /* largest index for which tbl[i] is valid */

    /* initialise: first five terms */
    if (!tbl) {
        cap   = 5;
        tbl   = alloc_seq_array(cap);
        for (size_t i = 0; i < 5; ++i) tbl[i] = (seq_t)1;
        max_n = 4;
    }

    /* if requested index already computed, return it */
    if (n <= max_n) return tbl[n];

    /* grow the table so it can hold index n */
    if ((size_t)n + 1 > cap)
        grow_seq_array(&tbl, &cap, (size_t)n + 1);

    /* compute missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val = 0;

        /* use only integer arithmetic on seq_t */
        val += (seq_t)5 * tbl[i - 1];
        val -= (seq_t)10 * tbl[i - 2];
        val += (seq_t)10 * tbl[i - 3];
        val -= (seq_t)5 * tbl[i - 4];
        val += (seq_t)3 * tbl[i - 5];

        tbl[i] = val;
    }

    max_n = n;
    return tbl[n];
}
