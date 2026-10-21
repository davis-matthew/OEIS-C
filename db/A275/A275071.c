/*
 * A275071  Number of set partitions of [n] such that i-j is a multiple of
 *          four for all i,j belonging to the same block.
 * https://oeis.org/A275071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=0..3} A000110(floor((n+i)/4))
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 24, u64 n <= 38, u128 n <= 62,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A275071:
   the product of four Bell numbers whose indices are
   floor(n/4), floor((n+1)/4), floor((n+2)/4) and floor((n+3)/4).  */
#include <stddef.h>                /* for size_t */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Largest Bell index needed for this n. */
    size_t need = (size_t)((n + 3) / 4);          /* floor((n+3)/4) */

    /* Static cache of Bell numbers up to the current capacity. */
    static seq_t *bell = NULL;
    static size_t bell_cap = 0;   /* number of allocated entries; bell[0]..bell[bell_cap-1] */

    /* Allocate or grow the cache if necessary. */
    if (need + 1 > bell_cap) {
        size_t old_cap = bell_cap;
        size_t new_cap = need + 1;

        seq_t *tmp;
        if (old_cap == 0) {
            tmp = seq_alloc(new_cap);   /* initialize array with zeros */
            if (!tmp) return (seq_t)0;  /* allocation failed */
            bell      = tmp;
            bell_cap  = new_cap;
            bell[0]   = (seq_t)1;       /* Bell(0) = 1 */
            old_cap   = 1;              /* next index to fill */
        } else {
            tmp = seq_realloc(bell, old_cap, new_cap);
            if (!tmp) return (seq_t)0;  /* allocation failed */
            bell     = tmp;
            bell_cap = new_cap;
            /* old_cap remains unchanged; indices 0..old_cap-1 are already set */
        }

        /* Compute Bell numbers for the newly added indices. */
        for (size_t i = old_cap; i < new_cap; ++i) {
            idx_t m = (idx_t)i - 1;        /* m = i-1, because Bell(i) depends on Bell(0..i-1) */
            seq_t sum  = 0;
            seq_t binom = 1;               /* C(m,0) */

            for (idx_t k = 0; k <= m; ++k) {
                sum += binom * bell[(size_t)k];
                if (k < m) {
                    binom = binom * (seq_t)(m - k);      /* multiply by (m-k) */
                    binom = binom / (seq_t)(k + 1);      /* divide by (k+1) */
                }
            }
            bell[(size_t)i] = sum;
        }
    }

    /* Compute the product of the four required Bell numbers. */
    seq_t result = 1;
    for (idx_t i = 0; i < 4; ++i) {
        idx_t idx = (n + i) / 4;     /* floor((n+i)/4) */
        result *= bell[(size_t)idx];
    }
    return result;
}
