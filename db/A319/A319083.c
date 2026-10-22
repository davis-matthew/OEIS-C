/*
 * A319083  Coefficients of polynomials related to the D'Arcais polynomials
 *          and Dedekind's eta(q) function, triangle read by rows, T(n,k) for
 *          0 <= k <= n.
 * https://oeis.org/A319083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term of the flattened triangle T(n,k) defined
 *            by T(0,0) = 1 For n > 0: T(n,0) = 0 T(n,k) = Σ_{m=k-1 .. n-1}
 *            σ(n-m) * T(m,k-1) (k≥1) where σ(x) is the sum of divisors of x
 *            (seq_t sigma_of(idx_t x)). The sequence A319083 is the list of
 *            T(n,k) read row‑wise: 1, 0,1, 0,3,1, 0,4,6,1,...
 * Range:     u8 n <= 30, u16 n <= 95, u32 n <= 285, u64 n <= 1008,
 *            u128 n <= 3775, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sigma_of.h"

/* Static cache for the triangle */
static seq_t **tri = NULL;       /* array of rows, tri[n][k] = T(n,k) */
static idx_t cached_rows = 0;    /* number of rows currently stored (tri[0]..tri[cached_rows-1]) */

/* Find the row r containing flattened index idx */
static idx_t find_row(idx_t idx)
{
    idx_t low = 0, high = 1;
    /* increase high until the triangular number exceeds idx */
    while (1) {
        __uint128_t t = (__uint128_t)(high + 1) * (high + 2) / 2;
        if (t > idx) break;
        high *= 2;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t t = (__uint128_t)(mid + 1) * (mid + 2) / 2;
        if (t <= idx)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

/* Ensure triangle rows 0..r are computed */
static void ensure_rows(idx_t r)
{
    if (r < cached_rows) return;

    /* Grow the array of row pointers if necessary */
    if (tri == NULL) {
        tri = (seq_t**)malloc((r + 1) * sizeof(seq_t*));
        cached_rows = 0;
    } else if (cached_rows <= r) {
        tri = (seq_t**)realloc(tri, (r + 1) * sizeof(seq_t*));
    }

    for (idx_t n = cached_rows; n <= r; ++n) {
        tri[n] = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (n == 0) {
            tri[0][0] = 1;
        } else {
            tri[n][0] = 0;
            for (idx_t k = 1; k <= n; ++k) {
                seq_t sum = 0;
                for (idx_t m = k - 1; m <= n - 1; ++m) {
                    seq_t sigma = sigma_of((idx_t)(n - m));
                    sum += sigma * tri[m][k - 1];
                }
                tri[n][k] = sum;
            }
        }
    }
    cached_rows = r + 1;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    /* Locate the row and column corresponding to flattened index n */
    idx_t r = find_row(n);
    __uint128_t prev = (__uint128_t)r * (r + 1) / 2;
    idx_t c = (idx_t)(((__uint128_t)n - prev));

    /* Ensure the required rows are computed */
    ensure_rows(r);

    return tri[r][c];
}
