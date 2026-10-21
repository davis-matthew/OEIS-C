/*
 * A255467  a(n) = A255466(2^n-1).
 * https://oeis.org/A255467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=6, a(2)=26; a(n)=5*a(n-1)-2*a(n-2)-8*a(n-3) for n>2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *mem = NULL;   /* cache of computed terms */
    static size_t cap = 0;      /* current capacity of the cache */

    size_t u = (size_t)n;        /* index as native unsigned type */

    /* Initialise the cache with the first three terms if not already done */
    if (cap == 0) {
        mem = (seq_t*)calloc(3, sizeof(seq_t));
        if (!mem) abort();
        mem[0] = (seq_t)1;
        mem[1] = (seq_t)6;
        mem[2] = (seq_t)26;
        cap = 3;
    }

    /* Grow the cache up to the required index */
    if (u >= cap) {
        size_t new_cap = u + 1;
        seq_t *tmp = (seq_t*)realloc(mem, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        mem = tmp;

        /* Compute and store missing terms using the recurrence */
        for (size_t i = cap; i <= u; ++i) {
            seq_t t = mem[i-1];
            t *= (seq_t)5;                     /* 5 * a(i-1) */
            t -= (seq_t)2 * mem[i-2];          /* - 2 * a(i-2) */
            t -= (seq_t)8 * mem[i-3];          /* - 8 * a(i-3) */
            mem[i] = t;
        }
        cap = new_cap;
    }

    return mem[u];
}
