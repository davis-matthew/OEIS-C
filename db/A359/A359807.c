/*
 * A359807  a(1) = 0; thereafter a(n) is the largest a(i) + i which is < n
 *          among i = 1..n-1.
 * https://oeis.org/A359807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A359807
   The definition:
     a(1) = 0
     a(n) = max{ a(i)+i | 1 ≤ i < n  and  a(i)+i < n }   (n > 1)
   The routine memoises the values so that calls with arbitrary
   indices are efficient. It uses only standard C facilities,
   so it works for any unsigned integer type seq_t defined by the
   build system, including arbitrary‑precision types. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *tab = NULL;          /* table of a[1..max] */
    static size_t alloc = 0;           /* allocated size */
    static idx_t max_computed = 0;     /* largest index stored */
    static idx_t m = 1;                /* first index that does NOT
                                           satisfy a(m)+m < current k  */

    if (n < 1)
        return (seq_t)0;               /* value for n=0 is irrelevant */

    /* Allocate or grow the table to be able to store indices up to n */
    if (alloc <= n) {
        size_t new_alloc = n + 1;      /* +1 because we ignore index 0 */
        seq_t *tmp = (seq_t *)realloc(tab, new_alloc * sizeof(seq_t));
        if (!tmp)
            exit(1);                  /* fatal error – out of memory */
        tab = tmp;
        alloc = new_alloc;
    }

    /* Initialise first term */
    if (max_computed < 1) {
        tab[1] = 0;
        max_computed = 1;
        m = 1;                        /* a(1)+1 = 1 < 2, so m = 1   */
    }

    /* If we already know a(n), return it */
    if (n <= max_computed)
        return tab[n];

    /* Compute new values up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* Move m forward while the condition holds */
        while (m < k && (tab[m] + (seq_t)m) < (seq_t)k)
            ++m;
        /* Now m-1 is the largest i with a(i)+i < k */
        idx_t j = m - 1;
        tab[k] = tab[j] + (seq_t)j;
    }

    max_computed = n;
    return tab[n];
}
