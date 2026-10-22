/*
 * A392316  a(n) = Sum_{k=0..floor(3*n/8)} binomial(k+2,2) *
 *          binomial(k,3*n-8*k).
 * https://oeis.org/A392316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(3*n/8)} binomial(k+2,2) *
 *            binomial(k,3*n-8*k)
 * Range:     u8 n <= 16, u16 n <= 33, u32 n <= 75, u64 n <= 159,
 *            u128 n <= 334, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp;

#ifdef __cplusplus
        /* C++: allocate a new array and copy old values */
        tmp = new seq_t[new_size];
        for (idx_t i = 0; i < cache_size; ++i)
            tmp[i] = cache[i];
        delete[] cache;
#else
        /* C: realloc the existing block */
        tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return 0;  /* allocation failed */
#endif

        /* compute missing terms */
        for (idx_t i = cache_size; i < new_size; ++i) {
            idx_t k_start = (i + 2) / 3;          /* ceil(i/3) */
            idx_t k_end   = (3 * i) / 8;          /* floor(3*i/8) */
            seq_t sum = 0;
            if (k_start <= k_end) {
                for (idx_t k = k_start; k <= k_end; ++k) {
                    idx_t m = 3 * i - 8 * k;        /* guaranteed 0 <= m <= k */
                    seq_t b1 = binomial(k + 2, 2);
                    seq_t b2 = binomial(k, m);
                    sum += b1 * b2;
                }
            }
            tmp[i] = sum;
        }

        cache = tmp;
        cache_size = new_size;
    }

    return cache[n];
}
