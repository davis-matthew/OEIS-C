/*
 * A287618  Triangle read by rows: T(j,k) is the number of distinct edge
 *          segments in a j X k rectangular grid.
 * https://oeis.org/A287618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, hear
 * Formula:   a(n) = if j==k: ceil(j/2) else ceil(j/2)+ceil(k/2) where j,k are
 *            the row/col indices for n in the triangular enumeration: n =
 *            (k-1)*k/2 + j, 1 <= j <= k
 * Range:     u8 n <= 9629, u16 n <= 9629, u32 n <= 9629, u64 n <= 9629,
 *            u128 n <= 9629, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t x)
{
    /* integer square root (floor) for 64‑bit unsigned integers */
    if (x == 0) return 0;
    idx_t r = 0;
    idx_t bit = (idx_t)1ULL << 62; /* highest power of 4 <= 2^64 */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* n >= 1, offset 1 */
    idx_t d = 1 + 8 * n;                 /* 1+8n */
    idx_t sqrt_d = isqrt_u64(d);          /* floor(sqrt(1+8n)) */
    idx_t k = (sqrt_d - 1) >> 1;          /* floor((-1+sqrt)/2) */

    /* adjust k upward if necessary (at most once) */
    if ((k * (k + 1)) / 2 < n) ++k;

    /* j = n - T(k-1) */
    idx_t j = n - ((k - 1) * k) / 2;

    seq_t sj = (seq_t)j;
    seq_t sk = (seq_t)k;

    seq_t term_j = (sj + 1) / 2;          /* ceil(j/2) */
    seq_t term_k = (sk + 1) / 2;          /* ceil(k/2) */

    if (j == k) return term_j;
    return term_j + term_k;
}
