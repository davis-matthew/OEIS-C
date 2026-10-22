/*
 * A305211  a(n) is the number of possible values of (x^3 + y^3) mod n, where
 *          x and y are any integers.
 * https://oeis.org/A305211
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

/* Computes a(n): the number of distinct residues that can be
   expressed as x^3 + y^3 (mod n) for any integers x and y.
   The algorithm is a brute‑force enumeration of all cubic residues
   modulo n and all their pairwise sums. */

/* The build system supplies the following types:
 *   idx_t – unsigned index type (uint64_t or larger)
 *   seq_t – unsigned integer type of arbitrary size (uint8_t, uint16_t,
 *           uint32_t, uint64_t, unsigned __int128, or a big‑int class).
 * No other declarations are required. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Public function required by the judge */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not defined for 0, guard */

    size_t N = (size_t)n;                 /* array size (fits in size_t) */

    /* Boolean array marking the cubic residues modulo n */
    bool *is_cube = (bool *)malloc(N * sizeof(bool));
    if (!is_cube) return 0;
    memset(is_cube, 0, N);

    /* Compute all distinct cubic residues */
    for (size_t i = 0; i < N; ++i) {
        /* Use a wide enough intermediate type for the cube computation */
        unsigned long long xi = (unsigned long long)i;
        unsigned long long ci = (xi * xi % n) * xi % n;   /* xi^3 mod n */
        is_cube[(size_t)ci] = true;
    }

    /* Build a compact list of the distinct cubic residues */
    idx_t *cube_vals = (idx_t *)malloc(N * sizeof(idx_t));
    if (!cube_vals) { free(is_cube); return 0; }
    size_t M = 0;
    for (size_t i = 0; i < N; ++i) {
        if (is_cube[i]) cube_vals[M++] = (idx_t)i;
    }

    /* Boolean array marking the attainable sums of two cubic residues */
    bool *is_sum = (bool *)malloc(N * sizeof(bool));
    if (!is_sum) {
        free(is_cube); free(cube_vals);
        return 0;
    }
    memset(is_sum, 0, N);

    /* Enumerate all pairwise sums */
    for (size_t i = 0; i < M; ++i) {
        unsigned long long a = (unsigned long long)cube_vals[i];
        for (size_t j = 0; j < M; ++j) {
            unsigned long long b = (unsigned long long)cube_vals[j];
            unsigned long long s = (a + b) % n;          /* sum mod n */
            is_sum[(size_t)s] = true;
        }
    }

    /* Count distinct residues represented by the sums */
    seq_t count = 0;
    for (size_t i = 0; i < N; ++i) if (is_sum[i]) ++count;

    /* Clean up */
    free(is_cube);
    free(is_sum);
    free(cube_vals);

    return count;
}
