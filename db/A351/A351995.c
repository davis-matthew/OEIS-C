/*
 * A351995  Square array A(n, k), n, k >= 0, read by antidiagonals upwards;
 *          A(n, k) = Sum_{ i >= 0 } b_i * 2^(k*i) where n = Sum_{ i >= 0 }
 *          b_i * 2^i.
 * https://oeis.org/A351995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = A(i,j) where i+j = d, * d = ⌊(√(8n+1)-1)/2⌋ * i = d - (n
 *            - d*(d+1)/2), * j = n - d*(d+1)/2 * and A(i,j) = Σ_{t≥0}
 *            (bit_i(t) · 2^{j·t}) with bit_i(t) the * t‑th bit of i
 *            (0‑based). This is the OEIS definition of A351995 * read by
 *            antidiagonals upwards
 * Range:     u8 n <= 39, u16 n <= 85, u32 n <= 200, u64 n <= 486,
 *            u128 n <= 1207, big any n
 * Helpers:   floor_isqrt_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/floor_isqrt_u64.h"

static inline seq_t A_ij(idx_t i, idx_t j)
{
    seq_t sum = 0;
    idx_t pos = 0;
    while (i != 0) {
        if (i & 1U)
            sum += pow2(j * pos);
        i >>= 1U;
        pos++;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Determine diagonal d such that the first n terms lie within
     * the first d+1 antidiagonals.  Count of terms up to diagonal d
     * is (d+1)(d+2)/2.  Solve for d using the floor of the quadratic root. */
    idx_t tmp = 8 * n + 1;
    idx_t s   = floor_isqrt_u64(tmp);
    idx_t d   = (s - 1) / 2;

    /* Count of terms before diagonal d */
    idx_t before = d * (d + 1) / 2;
    idx_t offset = n - before;

    /* Coordinates (i, j) of the element in the square array */
    idx_t i = d - offset;
    idx_t j = offset;

    return A_ij(i, j);
}
