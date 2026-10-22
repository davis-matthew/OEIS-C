/*
 * A330966  a(n) is 1/5 times the number of anti-chains of size four in
 *          "0,1,2" Motzkin trees on n edges.
 * https://oeis.org/A330966
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 16, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A330966
   a(n) = A335355(n)/5, offset 6.
   Recurrence:
      -(n+2)*(n-6)*a(n)
    + (n+2)*(4*n-17)*a(n-1)
    + (2*n^2-n-90)*a(n-2)
    - 3*(n+2)*(4*n-3)*a(n-3)
    - 9*(n+2)*(n+1)*a(n-4) = 0
   Initial values used (offset 6):
      a(6)=1, a(7)=11, a(8)=84, a(9)=520.
   The function computes values for any n ≥ 6 in O(1) per new term
   after an initial O(n0) fill, using a static cache so calls may
   arrive in any order. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/ensure_cache.h"  /* ensures cache_ptr size >= n+1 */

seq_t a(idx_t n) {
    if (n < 6) return 0;         /* value below the offset is irrelevant */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Ensure cache large enough for index n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialize known base terms. They are zero until set. */
    if (cache[6] == 0) {
        cache[6] = 1;
        cache[7] = 11;
        cache[8] = 84;
        cache[9] = 520;
    }

    /* Compute any missing a(i) up to n. */
    for (idx_t i = 6; i <= n; ++i) {
        if (cache[i] != 0) continue;   /* already computed */

        if (i < 10) { /* although we already set 6..9, this guard is safe */
            /* Should not happen, but keep base values. */
            switch (i) {
                case 6: cache[6] = 1; break;
                case 7: cache[7] = 11; break;
                case 8: cache[8] = 84; break;
                case 9: cache[9] = 520; break;
            }
            continue;
        }

        /* Recurrence computation. */
        seq_t N      = (seq_t)i;
        seq_t n_plus_2  = (seq_t)(i + 2);
        seq_t n_minus_6 = (seq_t)(i - 6);

        /* term1 = (n+2)*(4*n-17)*a(n-1) */
        seq_t term1 = n_plus_2 * ( (seq_t)(4*i - 17) );
        term1 *= cache[i-1];

        /* term2 = (2*n^2 - n - 90)*a(n-2) */
        seq_t term2 = (seq_t)(2*i*i - i - 90);
        term2 *= cache[i-2];

        /* term3 = 3*(n+2)*(4*n-3)*a(n-3) */
        seq_t term3 = (seq_t)3 * n_plus_2 * (seq_t)(4*i - 3);
        term3 *= cache[i-3];

        /* term4 = 9*(n+2)*(n+1)*a(n-4) */
        seq_t term4 = (seq_t)9 * n_plus_2 * (seq_t)(i + 1);
        term4 *= cache[i-4];

        seq_t numerator   = term1 + term2 - term3 - term4;
        seq_t denom       = n_plus_2 * n_minus_6;

        cache[i] = numerator / denom;
    }

    return cache[n];
}
