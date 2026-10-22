/*
 * A335925  a(n) = a(floor((n-1)/a(n-1))) + 1 with a(1) = 1.
 * https://oeis.org/A335925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(floor((n-1)/a(n-1))) + 1,   a(1) = 1 */
#include <stdlib.h>

/* cache[0] holds a(1), cache[n-1] holds a(n) */
static seq_t *cache = NULL;   /* dynamic array of computed values */
static idx_t cap  = 0;        /* current capacity of the array */

/* ensure that cache can hold at least n entries (1‑based) */
static void ensure_capacity(idx_t n)
{
    if (n <= cap)
        return;
    idx_t new_cap = cap ? cap * 2 : 16;
    while (new_cap < n)
        new_cap *= 2;
    /* realloc must be cast for C++ compilation */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);                       /* allocation failure */
    /* initialize the newly added part to 0 (sentinel) */
    for (idx_t i = cap; i < new_cap; ++i)
        tmp[i] = 0;
    cache = tmp;
    cap   = new_cap;
}

/* external linkage required by the specification */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined, guard clause */
    ensure_capacity(n);
    if (cache[n - 1] != 0)
        return cache[n - 1];
    if (n == 1) {                     /* base case */
        cache[0] = 1;
        return 1;
    }
    seq_t prev = a(n - 1);             /* compute a(n-1) */
    idx_t idx  = (n - 1) / (idx_t)prev;/* floor division */
    seq_t val  = a(idx) + 1;           /* recurrence */
    cache[n - 1] = val;
    return val;
}
