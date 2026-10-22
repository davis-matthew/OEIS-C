/*
 * A349611  Number of solutions to x^2 + y^2 + z^2 + w^2 <= n^2, where x, y,
 *          z, w are positive odd integers.
 * https://oeis.org/A349611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y,z,w) | x, y, z, w odd ≥ 1 and x² + y² + z² + w² ≤
 *            n²}
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 687, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   compare_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/compare_uint32.h"

seq_t a(idx_t n)
{
    /* Number of positive odd integers ≤ n */
    idx_t m = (n + 1) / 2;        /* ceil(n/2) */
    if (m == 0)
        return (seq_t)0;

    /* Size of array of two‑odd‑square sums: m² */
    size_t Tsize = (size_t)m * (size_t)m;
    if (Tsize == 0)
        return (seq_t)0;

    /* Allocate and fill the array */
    uint32_t *T = (uint32_t *)malloc(Tsize * sizeof(uint32_t));
    if (!T)
        return (seq_t)0; /* allocation failure => return 0 */

    size_t pos = 0;
    for (idx_t ix = 0; ix < m; ++ix) {
        uint32_t x = 1u + 2u * (uint32_t)ix;          /* odd values: 1,3,5,… */
        uint32_t x2 = x * x;
        for (idx_t iy = 0; iy < m; ++iy) {
            uint32_t y = 1u + 2u * (uint32_t)iy;
            uint32_t y2 = y * y;
            T[pos++] = x2 + y2;
        }
    }

    /* Sort the sums */
    qsort(T, Tsize, sizeof(uint32_t), compare_uint32);

    /* Precompute n² */
    unsigned long long n2 = (unsigned long long)n * (unsigned long long)n;

    /* Two‑pointer counting */
    long long i = (long long)Tsize - 1;   /* furthest index that satisfies the limit */
    seq_t result = (seq_t)0;

    for (size_t j = 0; j < Tsize; ++j) {
        long long limit = (long long)n2 - (long long)T[j];
        while (i >= 0 && (long long)T[i] > limit)
            --i;
        if (i >= 0)
            result += (seq_t)(i + 1);      /* indices 0 … i inclusive satisfy the limit */
    }

    free(T);
    return result;
}
