/*
 * A351825  Total number of size 2 lists in all sets of lists partitioning [n]
 *          (cf. A000262).
 * https://oeis.org/A351825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*binomial(n,2)*A000262(n-2) Recurrence: (n-2)*a(n) =
 *            n*(2*n-5)*a(n-1) - (n-4)*(n-1)*n*a(n-2) with a(0)=0, a(1)=0,
 *            a(2)=2, a(3)=6
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        cache = tmp;
        cache_size = new_size;

        /* Initialize the base values */
        if (old_size == 0) {
            if (new_size > 0) cache[0] = 0;
            if (new_size > 1) cache[1] = 0;
            if (new_size > 2) cache[2] = 2;
        } else {
            if (old_size <= 1 && new_size > 1) cache[1] = 0;
            if (old_size <= 2 && new_size > 2) cache[2] = 2;
        }
        if (new_size > 3) cache[3] = 6;

        idx_t start = old_size > 3 ? old_size : 4;
        for (idx_t i = start; i <= n; ++i) {
#ifdef __cplusplus
            /* Big‑int arithmetic (C++ mode) */
            seq_t a_nm1 = cache[i-1];
            seq_t a_nm2 = cache[i-2];
            seq_t term1 = (seq_t)i * (seq_t)(2*i - 5) * a_nm1;
            seq_t term2 = (seq_t)(i-4) * (seq_t)(i-1) * (seq_t)i * a_nm2;
            cache[i] = (term1 - term2) / (seq_t)(i-2);
#else
            /* Fixed‑width integer mode (C mode) – use 128‑bit intermediates */
            long long ii = (long long)i;
            __int128 term1 = (__int128)ii * (2*ii - 5) * (__int128)cache[i-1];
            __int128 term2 = (__int128)(ii-4) * (ii-1) * ii * (__int128)cache[i-2];
            __int128 num = term1 - term2;
            cache[i] = (seq_t)(num / (ii-2));
#endif
        }
    }
    return cache[n];
}
