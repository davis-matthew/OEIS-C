/*
 * A320764  Number of ordered set partitions of [n] where the maximal block
 *          size equals eight.
 * https://oeis.org/A320764
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered set partitions of [n] where the maximal
 *            block size equals eight. Computed as a(n) = F_8(n) – F_7(n),
 *            where F_m(n) = Σ_{k=1..min(m,n)} binom(n,k) * F_m(n–k), F_m(0) =
 *            1. This recursive relation is used for all n ≥ 0
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 15, u64 n <= 21, u128 n <= 33,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/binom.h"   /* declares seq_t binom(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    /* Static caches for F_7 and F_8; cache_size is the largest n cached. */
    static seq_t *F7 = NULL;
    static seq_t *F8 = NULL;
    static idx_t cache_size = 0;   /* 0 means no data yet */

    /* Extend cache to cover n if needed */
    if (n > cache_size) {
        /* Allocate new arrays of size n+1 */
        idx_t new_size = n + 1;
        seq_t *new_f7 = (seq_t *)malloc(new_size * sizeof(seq_t));
        seq_t *new_f8 = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_f7 || !new_f8) {
            abort(); /* allocations failed */
        }

        /* Copy existing data or initialize base case */
        if (cache_size > 0) {
            memcpy(new_f7, F7, (cache_size + 1) * sizeof(seq_t));
            memcpy(new_f8, F8, (cache_size + 1) * sizeof(seq_t));
        } else {
            new_f7[0] = (seq_t)1;
            new_f8[0] = (seq_t)1;
        }

        /* Free old arrays */
        free(F7);
        free(F8);

        F7 = new_f7;
        F8 = new_f8;

        /* Compute new entries from cache_size+1 up to n */
        idx_t start = (cache_size == 0 ? 1 : cache_size + 1);
        for (idx_t i = start; i <= n; ++i) {
            seq_t sum7 = 0;
            seq_t sum8 = 0;

            idx_t limit7 = (i < 7) ? i : 7;
            idx_t limit8 = (i < 8) ? i : 8;

            for (idx_t j = 1; j <= limit7; ++j) {
                sum7 += binom(i, j) * F7[i - j];
            }
            for (idx_t j = 1; j <= limit8; ++j) {
                sum8 += binom(i, j) * F8[i - j];
            }

            F7[i] = sum7;
            F8[i] = sum8;
        }

        cache_size = n;
    }

    /* For n < 8 the difference is zero; otherwise return the required value. */
    return F8[n] - F7[n];
}
