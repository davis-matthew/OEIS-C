/*
 * A306399  a(1)=1; a(n) = number of occurrences of a(n-1) if a(n-1) is odd;
 *          a(n) = number of occurrences of a(n-2) if a(n-1) is even.
 * https://oeis.org/A306399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 703, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A306399
 *
 * a(1) = 1,  a(2) = 1
 * For n > 2:
 *   if a(n-1) is odd  →  a(n) = number of occurrences of a(n-1)
 *   if a(n-1) is even →  a(n) = number of occurrences of a(n-2)
 *
 * The implementation memoises previously computed values in a static
 * dynamically‑grown array.  Counting the occurrences of the target
 * value is performed by scanning the already known part of the array,
 * giving an overall O(n²) worst‑case running time which is easily fast
 * enough for the required limits.
 */

#include <stddef.h>
#include <stdlib.h>

/* external linkage: seq_t and idx_t are defined elsewhere by the build */
seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *seq     = NULL;     /* 1‑based array of terms */
    static size_t  cap    = 0;        /* current allocated capacity */
    static idx_t   max_n  = 0;        /* largest n that has been computed */

    if (n == 0)
        return (seq_t)0;              /* undefined, but safe guard */

    /* ensure array can hold indices up to n (1‑based) */
    if (n+1 > cap) {
        size_t new_cap = cap ? cap << 1 : 1;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;            /* grow geometrically */
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* initialise first terms on first use */
    if (max_n == 0) {
        seq[1] = (seq_t)1;           /* a(1) */
        max_n = 1;
    }

    /* compute all missing terms up to n */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t target;

        if (k == 1) {                /* already initialised, but kept for clarity */
            seq[1] = (seq_t)1;
            continue;
        }

        if (k == 2) {                /* base value */
            seq[2] = (seq_t)1;
            continue;
        }

        /* choose reference value per the parity rule */
        seq_t prev = seq[k - 1];
        if (prev & (seq_t)1)         /* odd */
            target = prev;
        else                         /* even */
            target = seq[k - 2];

        /* count how many times target has appeared in indices 1 .. k-1 */
        idx_t cnt = 0;
        for (idx_t i = 1; i < k; ++i)
            if (seq[i] == target)
                ++cnt;

        seq[k] = (seq_t)cnt;
    }

    max_n = n;
    return seq[n];
}
