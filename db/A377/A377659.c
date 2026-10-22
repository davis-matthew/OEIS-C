/*
 * A377659  a(n) = Motzkin(n) - 2^(n - 1 + 0^n) = A001006(n) - A011782(n).
 * https://oeis.org/A377659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Motzkin(n) - 2^(n-1) (for n > 0; a(0) = 0)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 45, u128 n <= 85,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* realloc, NULL */
#include "../utils/pow2.h"   /* pow2(seq_t) */

seq_t a(idx_t n)
{
    /* Special case */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Static cache of Motzkin numbers */
    static seq_t *M = NULL;
    static size_t cap = 0;   /* number of entries stored */

    /* Grow the array if necessary */
    if (cap <= n) {
        size_t old_cap = cap;
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(M, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – fail silently */
            return (seq_t)0;
        }
        M = tmp;

        /* Initialise first two Motzkin numbers if not already done */
        if (old_cap == 0) {
            M[0] = (seq_t)1;
            M[1] = (seq_t)1;
            old_cap = 2;
        }

        /* Compute missing Motzkin numbers up to n */
        for (idx_t i = (idx_t)old_cap; i <= n; ++i) {
            if (sizeof(seq_t) <= 8) {
                /* 64‑bit or smaller – use 128‑bit intermediates */
                __int128 term1 = (__int128)M[i - 1] * (2 * i + 1);
                __int128 term2 = (__int128)M[i - 2] * (3 * (i - 1));
                __int128 numer = term1 + term2;
                M[i] = (seq_t)(numer / (i + 2));
            } else {
                /* Arbitrary‑precision – rely on seq_t arithmetic */
                seq_t term1 = M[i - 1] * (seq_t)(2 * i + 1);
                seq_t term2 = M[i - 2] * (seq_t)(3 * (i - 1));
                M[i] = (term1 + term2) / (i + 2);
            }
        }
        cap = new_cap;
    }

    seq_t mot = M[n];
    seq_t pow = pow2(n - 1);   /* 2^(n-1) for n > 0 */

    return mot - pow;
}
