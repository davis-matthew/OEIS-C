/*
 * A366870  a(0) = 0; a(n) = n - a(floor(a(n-1)/4)).
 * https://oeis.org/A366870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = n - a(floor(a(n-1)/4))
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A366870.
   Memoizes results in a static dynamic array to handle
   arbitrary call order and large n.  Compatible with C11
   and C++17.  The type seq_t may be an arbitrary‑precision
   integer class; the same code works in either language. */

#include <stdlib.h> /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static dynamic cache */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;   /* size of allocated array */
    static idx_t max_computed = 0; /* highest index computed so far */

    /* Allocate initial cache if necessary */
    if (capacity == 0) {
        capacity = 16;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out‑of‑memory fallback */
        cache[0] = (seq_t)0;                 /* a(0) = 0 */
        max_computed = 0;
    }

    /* Ensure we have space up to n */
    if (capacity <= n) {
        idx_t newcap = capacity ? capacity : 16;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* out‑of‑memory fallback */
        cache = tmp;
        capacity = newcap;
    }

    /* Compute any missing values from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t div4 = prev / (seq_t)4;             /* floor(a(i-1)/4) */
        idx_t idx = (idx_t)div4;  /* safe: div4 <= i-1 */
        cache[i] = (seq_t)i - cache[idx];
    }
    max_computed = n;

    return cache[n];
}
