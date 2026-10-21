/*
 * A266671  Decimal representation of the n-th iteration of the "Rule 53"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25*a(n-2) - 152*a(n-4) + 128*a(n-6) (for n>5) with
 *            a(0)=1, a(1)=3, a(2)=8, a(3)=95, a(4)=64, a(5)=1791
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute the n-th term of OEIS A266671. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;   /* array of terms */
    static size_t  cap   = 0;      /* allocated capacity (in terms) */
    static idx_t   len   = 0;      /* number of terms currently stored */

    /* Ensure enough capacity for indices up to n. */
    if (n + 1 > cap) {
        size_t newcap = cap ? (size_t)cap << 1 : 1;
        while (newcap <= (size_t)n) {
            newcap <<= 1;
        }
        if (newcap < 6) newcap = 6;               /* at least space for 0..5 */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        /* If allocation fails, return 0 (undefined behaviour ignored) */
        if (tmp == NULL) return (seq_t)0;
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise the first six terms once. */
    if (len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)95;
        cache[4] = (seq_t)64;
        cache[5] = (seq_t)1791;
        len = 6;
    }

    /* If the requested term is already cached, return it. */
    if (n < len) {
        return cache[n];
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = len; i <= n; ++i) {
        /* i >= 6 ensures all indices in the recurrence are valid. */
        cache[i] = ((seq_t)25) * cache[i - 2]
                 - ((seq_t)152) * cache[i - 4]
                 + ((seq_t)128) * cache[i - 6];
    }
    len = n + 1;
    return cache[n];
}
